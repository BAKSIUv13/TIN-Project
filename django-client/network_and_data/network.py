# TIN
# Bartlomiej Kulik

"""Module responsible for network layer."""

import socket

HOST = 'localhost'
PORT = 12345

def prepare_socket(host, port):
    """Create socket and try to connect."""
    _s = socket.socket(socket.AF_INET,
                       socket.SOCK_STREAM,
                       socket.IPPROTO_TCP)
    _s.connect((host, port)) # tuple because of AF_INET address format

    return _s
