# TIN
# Bartłomiej Kulik

"""Module responsible for creating socket, necessary pipes and conversions."""

import os
import socket


def prepare_socket(host, port):
    """Create socket and try to connect. Return createdf socket."""
    # create_connection = socket() + connect()
    client_socket = socket.create_connection((host, port))

    return client_socket

def prepare_error_pipe():
    """Return (read_pipe, write_pipe) pipes to read and write error messages."""
    r_fd, w_fd = os.pipe()

    return (os.fdopen(r_fd), os.fdopen(w_fd, 'w'))
