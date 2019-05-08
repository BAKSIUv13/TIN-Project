

import socket


def prepare_socket(host, port):
    """Create socket and try to connect. Return createdf socket."""
    # create_connection = socket() + connect()
    client_socket = socket.create_connection((host, port))

    return client_socket

