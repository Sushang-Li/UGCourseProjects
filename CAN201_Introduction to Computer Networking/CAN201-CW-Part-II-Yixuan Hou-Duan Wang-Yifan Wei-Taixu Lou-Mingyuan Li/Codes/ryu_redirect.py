from ryu.base import app_manager
from ryu.controller import ofp_event
from ryu.controller.handler import CONFIG_DISPATCHER, MAIN_DISPATCHER
from ryu.controller.handler import set_ev_cls
from ryu.ofproto import ofproto_v1_3

from ryu.lib.packet import packet
from ryu.lib.packet import ethernet
from ryu.lib.packet import ether_types
from ryu.lib.packet import in_proto
from ryu.lib.packet import ipv4
from ryu.lib.packet import icmp
from ryu.lib.packet import tcp
from ryu.lib.packet import udp

client_ip = '10.0.1.5'
server_1_ip = '10.0.1.2'
server_2_ip = '10.0.1.3'

client_mac = '00:00:00:00:00:03'
server_1_mac = '00:00:00:00:00:01'
server_2_mac = '00:00:00:00:00:02'


class SimpleSwitch13(app_manager.RyuApp):
    # use OpenFlow 1.3
    OFP_VERSIONS = [ofproto_v1_3.OFP_VERSION]

    def __init__(self, *args, **kwargs):
        super(SimpleSwitch13, self).__init__(*args, **kwargs)
        self.mac_to_port = {}

    @set_ev_cls(ofp_event.EventOFPSwitchFeatures, CONFIG_DISPATCHER)
    def switch_features_handler(self, ev):
        datapath = ev.msg.datapath
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser

        # match all package
        match = parser.OFPMatch()

        # deal with all table-miss flow entries
        actions = [parser.OFPActionOutput(ofproto.OFPP_CONTROLLER, ofproto.OFPCML_NO_BUFFER)]

        self.add_flow(datapath, 0, match, actions)

    def add_flow(self, datapath, priority, match, actions, buffer_id=None):
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser

        inst = [parser.OFPInstructionActions(ofproto.OFPIT_APPLY_ACTIONS, actions)]

        if buffer_id:
            mod = parser.OFPFlowMod(datapath=datapath, buffer_id=buffer_id, priority=priority, idle_timeout=5,
                                    match=match, instructions=inst)
        elif priority == 0:
            mod = parser.OFPFlowMod(datapath=datapath, priority=priority, match=match, instructions=inst)
        else:
            mod = parser.OFPFlowMod(datapath=datapath, priority=priority, idle_timeout=5, match=match,
                                    instructions=inst)

        datapath.send_msg(mod)

    # deal with PacketIn event
    @set_ev_cls(ofp_event.EventOFPPacketIn, MAIN_DISPATCHER)
    def _packet_in_handler(self, ev):
        if ev.msg.msg_len < ev.msg.total_len:
            self.logger.debug("packet truncated: only %s of %s bytes", ev.msg.msg_len, ev.msg.total_len)

        msg = ev.msg
        datapath = msg.datapath
        ofproto = datapath.ofproto
        parser = datapath.ofproto_parser
        in_port = msg.match['in_port']

        pkt = packet.Packet(msg.data)
        eth = pkt.get_protocols(ethernet.ethernet)[0]

        if eth.ethertype == ether_types.ETH_TYPE_LLDP:
            # ignore lldp packet
            return

        eth_src = eth.src  # src MAC address
        eth_dst = eth.dst  # dst MAC address

        dpid = format(datapath.id, "d").zfill(16)
        self.mac_to_port.setdefault(dpid, {})
        self.logger.info("PackIn Event\n"
                         "  dpid=%s\n"
                         "  src=%s\n"
                         "  dst=%s\n"
                         "  in_port=%s\n",
                         dpid, eth_src, eth_dst, in_port)

        self.mac_to_port[dpid][eth_src] = in_port
        self.logger.info("mac_to_port Table %s \n", self.mac_to_port)

        if eth_dst in self.mac_to_port[dpid]:
            out_port = self.mac_to_port[dpid][eth_dst]
        else:
            out_port = ofproto.OFPP_FLOOD

        actions = [parser.OFPActionOutput(out_port)]
        # if the output port is not flooding
        if out_port != ofproto.OFPP_FLOOD:
            if eth.ethertype == ether_types.ETH_TYPE_IP:
                pkt_ipv4 = pkt.get_protocol(ipv4.ipv4)
                ip_src = pkt_ipv4.src
                ip_dst = pkt_ipv4.dst
                ip_protocol = pkt_ipv4.proto

                # if ICMP Protocol
                if ip_protocol == in_proto.IPPROTO_ICMP:
                    match = parser.OFPMatch(eth_type=ether_types.ETH_TYPE_IP, in_port=in_port, ipv4_src=ip_src,
                                            ipv4_dst=ip_dst, ip_proto=ip_protocol)

                # if TCP Protocol
                elif ip_protocol == in_proto.IPPROTO_TCP:
                    # client send message to server1 -> controller should change sending MAC address to server2's MAC address
                    if ip_src == client_ip and ip_dst == server_1_ip:
                        # if the output port corresponding to server2's MAC address exists in mac_to_port[dpid]:
                        if server_2_mac in self.mac_to_port[dpid]:
                            out_port = self.mac_to_port[dpid][server_2_mac]
                        else:
                            out_port = ofproto.OFPP_FLOOD
                        # in "match", destination IP address is still be server1's IP, but the output port is pointing to server2
                        match = parser.OFPMatch(eth_type=ether_types.ETH_TYPE_IP, ipv4_src=ip_src, ipv4_dst=ip_dst)

                        # actions should change destination address to server2's MAC address, destination IP address to server2's IP address
                        # output port should point to server2
                        actions = [parser.OFPActionSetField(eth_dst=server_2_mac),
                                   parser.OFPActionSetField(ipv4_dst=server_2_ip),
                                   parser.OFPActionOutput(port=out_port)]

                    # server2 send message to client
                    elif ip_src == server_2_ip and ip_dst == client_ip:
                        # if the output port corresponding to client's MAC address exists in mac_to_port[dpid]:
                        if client_mac in self.mac_to_port[dpid]:
                            out_port = self.mac_to_port[dpid][client_mac]
                        else:
                            out_port = ofproto.OFPP_FLOOD
                        # in "match", source IP address is server2's IP address, destination IP address is client's IP address, and the output port should point to client
                        match = parser.OFPMatch(eth_type=ether_types.ETH_TYPE_IP, ipv4_src=ip_src, ipv4_dst=ip_dst)

                        # actions should change source MAC address, source IP address to server1's and output port should point to client
                        actions = [parser.OFPActionSetField(eth_src=server_1_mac),
                                   parser.OFPActionSetField(ipv4_src=server_1_ip),
                                   parser.OFPActionOutput(port=out_port)]

                    else:
                        match = parser.OFPMatch(eth_type=ether_types.ETH_TYPE_IP, in_port=in_port, ipv4_dst=ip_dst,
                                                ip_proto=ip_protocol)

            elif eth.ethertype == ether_types.ETH_TYPE_ARP:
                match = parser.OFPMatch(eth_type=ether_types.ETH_TYPE_ARP, in_port=in_port, eth_dst=eth_dst,
                                        eth_src=eth_src)

            if msg.buffer_id != ofproto.OFP_NO_BUFFER:
                self.add_flow(datapath, 1, match, actions, msg.buffer_id)
                return
            else:
                self.add_flow(datapath, 1, match, actions)

        data = None
        if msg.buffer_id == ofproto.OFP_NO_BUFFER:
            data = msg.data

        out = parser.OFPPacketOut(datapath=datapath, buffer_id=msg.buffer_id, in_port=in_port, actions=actions,
                                  data=data)
        self.logger.info("PackOut Event\n"
                         "  dpid=%s\n"
                         "  in_port=%s\n"
                         "  actions=%s\n"
                         "  buffer_id=%s\n",
                         dpid, in_port, actions, msg.buffer_id)

        datapath.send_msg(out)