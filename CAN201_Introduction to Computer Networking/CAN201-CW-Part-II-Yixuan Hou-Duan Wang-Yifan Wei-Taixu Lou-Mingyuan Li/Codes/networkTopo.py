from mininet.net import Mininet
from mininet.cli import CLI
from mininet.node import Host
from mininet.node import OVSKernelSwitch
from mininet.log import setLogLevel,info
from mininet.node import RemoteController
from mininet.term import makeTerm

def myTopo():
    net = Mininet(topo=None, autoSetMacs=True, build=False, ipBase='10.0.1.0/24')

    c1 = net.addController('c1',RemoteController)

    h1 = net.addHost('server1',cls=Host, defaultRoute=None)
    h2 = net.addHost('server2',cls=Host, defaultRoute=None)
    h3 = net.addHost('client',cls=Host, defaultRoute=None)

    s1 = net.addSwitch('s1',cls=OVSKernelSwitch,failMode='secure')

    net.addLink(h1, s1)
    net.addLink(h2, s1)
    net.addLink(h3, s1)

    net.build()

    h1.setMAC(intf='server1-eth0',mac="00:00:00:00:00:01")
    h2.setMAC(intf='server2-eth0',mac="00:00:00:00:00:02")
    h3.setMAC(intf='client-eth0',mac="00:00:00:00:00:03")

    h1.setIP(intf="server1-eth0",ip='10.0.1.2/24')
    h2.setIP(intf="server2-eth0",ip='10.0.1.3/24')
    h3.setIP(intf="client-eth0",ip='10.0.1.5/24')

    net.start()

    net.terms += makeTerm(c1)
    net.terms += makeTerm(s1)
    net.terms += makeTerm(h1)
    net.terms += makeTerm(h2)
    net.terms += makeTerm(h3)

    CLI(net)
    net.stop()

if __name__ == '__main__':
    setLogLevel('info')
    myTopo()

