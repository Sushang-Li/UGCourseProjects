import hashlib
import json
import os
import struct
from socket import *
import argparse
import time

MAX_PACKET_SIZE = 20480

# Const Value
OP_SAVE, OP_DELETE, OP_GET, OP_UPLOAD, OP_DOWNLOAD, OP_BYE, OP_LOGIN, OP_ERROR = 'SAVE', 'DELETE', 'GET', 'UPLOAD', 'DOWNLOAD', 'BYE', 'LOGIN', 'ERROR'
TYPE_FILE, TYPE_DATA, TYPE_AUTH, DIR_EARTH = 'FILE', 'DATA', 'AUTH', 'EARTH'
FIELD_OPERATION, FIELD_DIRECTION, FIELD_TYPE, FIELD_USERNAME, FIELD_PASSWORD, FIELD_TOKEN = 'operation', 'direction', 'type', 'username', 'password', 'token'
FIELD_KEY, FIELD_SIZE, FIELD_TOTAL_BLOCK, FIELD_MD5, FIELD_BLOCK_SIZE = 'key', 'size', 'total_block', 'md5', 'block_size'
FIELD_STATUS, FIELD_STATUS_MSG, FIELD_BLOCK_INDEX = 'status', 'status_msg', 'block_index'
DIR_REQUEST, DIR_RESPONSE = 'REQUEST', 'RESPONSE'

server_port = 1379


def get_file_md5(filename):
    """
    get file md5 value of a file
    :param filename:
    :return: md5 value of a file
    """
    m = hashlib.md5()
    with open(filename, 'rb') as fid:
        while True:
            d = fid.read(2048)
            if not d:
                break
            m.update(d)
    return m.hexdigest()


def make_packet(json_data, bin_data=None):
    """
    make a packet based on STEP protocol
    :param json_data:
    :param bin_data:
    :return: a packet whose format is following STEP protocol
    """
    j = json.dumps(dict(json_data), ensure_ascii=False)
    j_len = len(j)
    if bin_data is None:
        return struct.pack('!II', j_len, 0) + j.encode()
    else:
        return struct.pack('!II', j_len, len(bin_data)) + j.encode() + bin_data


def make_request_packet(operation, data_type, json_data, bin_data=None):
    """
    make a request packet
    :param operation:
    :param data_type:
    :param json_data:
    :param bin_data:
    :return: a request packet
    """
    json_data[FIELD_OPERATION] = operation
    json_data[FIELD_DIRECTION] = DIR_REQUEST
    json_data[FIELD_TYPE] = data_type
    return make_packet(json_data, bin_data)


def send_packet(connection_socket, operation, data_type, json_data, bin_data=None):
    """
    send  packet to server
    :param connection_socket:
    :param operation:
    :param data_type:
    :param json_data:
    :param bin_data:
    :return: None
    """
    request_packet = make_request_packet(operation, data_type, json_data, bin_data)
    connection_socket.send(request_packet)


def get_tcp_packet(conn):
    """
    use client socket to get tcp_packet and return it
    :param conn:
    :return: json_data and bin_data
    """
    bin_data = b''
    while len(bin_data) < 8:
        data_rec = conn.recv(8)
        if data_rec == b'':
            time.sleep(0.01)
        if data_rec == b'':
            return None, None
        bin_data += data_rec

    data = bin_data[:8]
    bin_data = bin_data[8:]
    j_len, b_len = struct.unpack('!II', data)

    while len(bin_data) < j_len:
        data_rec = conn.recv(j_len)
        if data_rec == b'':
            time.sleep(0.01)
        if data_rec == b'':
            return None, None
        bin_data += data_rec
    j_bin = bin_data[:j_len]

    try:
        json_data = json.loads(j_bin.decode())
    except Exception as ex:
        return None, None

    bin_data = bin_data[j_len:]

    while len(bin_data) < b_len:
        data_rec = conn.recv(b_len)
        if data_rec == b'':
            time.sleep(0.01)
        if data_rec == b'':
            return None, None
        bin_data += data_rec

    return json_data, bin_data


def login(connection_socket, username, password):
    """
    achieve login function
    :param connection_socket:
    :param username:
    :param password:
    :return: server response packet
    """
    json_data = {
        FIELD_USERNAME: username,
        FIELD_PASSWORD: password
    }

    send_packet(connection_socket, OP_LOGIN, TYPE_AUTH, json_data)
    response, _ = get_tcp_packet(connection_socket)
    return response


def get_authorization_token(connection_socket, username, password):
    """
    get authorization token from server
    :param connection_socket:
    :param username:
    :param password:
    :return: token field of repsonse packet and response packet
    """
    response = login(connection_socket, username, password)

    if response[FIELD_OPERATION] == OP_LOGIN and response[FIELD_STATUS] == 200:
        return response[FIELD_TOKEN], response
    else:
        return None


def request_upload_plan(connection_socket, size, token, key=None):
    """
    request upload plan
    :param connection_socket:
    :param size:
    :param token:
    :param key:
    :return: server resopnse packet
    """
    if key is None:
        json_data = {
            FIELD_SIZE: size,
            FIELD_TOKEN: token
        }
    else:
        json_data = {
            FIELD_KEY: key,
            FIELD_SIZE: size,
            FIELD_TOKEN: token
        }

    send_packet(connection_socket, OP_SAVE, TYPE_FILE, json_data)
    response, _ = get_tcp_packet(connection_socket)
    print(response)
    # return response
    if response[FIELD_OPERATION] == OP_SAVE and response[FIELD_STATUS] == 200:
        return response
    else:
        return None



def upload_file_block(connection_socket, key, total_block, block_index, data, token):
    """
    upload file block to server
    :param connection_socket:
    :param key:
    :param total_block:
    :param block_index:
    :param data:
    :param token:
    :return: server response packet
    """
    json_data = {
        FIELD_KEY: key,
        FIELD_BLOCK_INDEX: block_index,
        FIELD_TOTAL_BLOCK: total_block,
        FIELD_TOKEN: token
    }

    send_packet(connection_socket, OP_UPLOAD, TYPE_FILE, json_data, data)
    response, _ = get_tcp_packet(connection_socket)

    if response[FIELD_OPERATION] == OP_UPLOAD and response[FIELD_STATUS] == 200:
        return response
    else:
        return None


def main():
    parser = argparse.ArgumentParser(description="Client for file upload")
    parser.add_argument("--server_ip", required=True, help="Server IP address")
    parser.add_argument("--id", required=True, help="the id")
    parser.add_argument("--f", required=True, help="Path to the file to upload")
    args = parser.parse_args()
    server_ip = args.server_ip
    username = args.id
    file_path = args.f

    client_socket = socket(AF_INET, SOCK_STREAM)
    client_socket.connect((server_ip, server_port))

    password = hashlib.md5(username.encode()).hexdigest()

    # Get authorization token
    token, r = get_authorization_token(client_socket, username, password)
    if token is None:
        print("Login failed.")
        return
    print(r)

    # Request upload plan
    response = request_upload_plan(client_socket, os.path.getsize(file_path), token)

    if response is None:
        print("Failed to request upload plan.")
        return

    total_blocks = response[FIELD_TOTAL_BLOCK]
    block_size = response[FIELD_BLOCK_SIZE]
    key = response[FIELD_KEY]

    # upload the file block by block
    with open(file_path, 'rb') as file:
        for block_index in range(total_blocks):
            data = file.read(block_size)
            response = upload_file_block(client_socket, key, total_blocks, block_index, data, token)
            if response is None:
                print(f"Failed to upload block {block_index}.")
                return
            print(response)

    # check the status of the file on the server according to the protocol
    if response[FIELD_STATUS] == 200:
        print("File has been uploaded successfully.")

    # check whether the file is received by the server properly
    print("Compare with the local MD5: ", get_file_md5(file_path))
    if response[FIELD_MD5] == get_file_md5(file_path):
        print("The file is received by the server properly.")
        return


if __name__ == "__main__":
    main()
