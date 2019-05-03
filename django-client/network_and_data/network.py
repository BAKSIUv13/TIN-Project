# TIN
# Bartlomiej Kulik

"""Module responsible for network layer."""

import socket

HOST = 'localhost'
PORT = 12345

def prepare_client_socket(host, port):
    """Create socket and try to connect."""
    client_socket = socket.socket(socket.AF_INET,
                                  socket.SOCK_STREAM,
                                  socket.IPPROTO_TCP)
    # tuple because of AF_INET address format
    client_socket.connect((host, port))

    return client_socket
