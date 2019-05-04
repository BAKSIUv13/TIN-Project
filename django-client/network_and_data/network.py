# TIN
# Bartlomiej Kulik

"""Module responsible for network layer."""

import socket

HOST = 'localhost'
PORT = 12345

def prepare_client_socket(host=HOST, port=PORT):
    """Create socket and try to connect."""
    # create_connection = socket() + connect()
    client_socket = socket.create_connection((host, port))

    return client_socket
