# TIN
# Bartlomiej Kulik

"""Module responsible for network layer."""

import socket
import os

HOST = 'localhost'
PORT = 12345

def prepare_socket(host=HOST, port=PORT):
    """Create socket and try to connect."""
    # create_connection = socket() + connect()
    client_socket = socket.create_connection((host, port))

    return client_socket

def prepare_error_network_pipe():
    """Return (read_pipe, write_pipe) pipes to read and write error messages."""
    r_fd, w_fd = os.pipe()

    return (os.fdopen(r_fd), os.fdopen(w_fd, 'w'))
