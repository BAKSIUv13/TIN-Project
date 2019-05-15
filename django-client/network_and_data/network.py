# TIN
# Bartłomiej Kulik

"""Module responsible for creating socket, necessary pipes and conversions."""

import socket
import os

def prepare_socket(host, port):
    """Create socket and try to connect. Return createdf socket."""
    # create_connection = socket() + connect()
    client_socket = socket.create_connection((host, port))

    return client_socket

def prepare_error_pipe():
    """Return (read_pipe, write_pipe) pipes to read and write error messages."""
    r_fd, w_fd = os.pipe()

    return (os.fdopen(r_fd), os.fdopen(w_fd, 'w'))

# conversion

def int_to_byte_array_4(int_value):
    """Convert int_value to 4 element byte array in big endian order."""
    return int_value.to_bytes(length=4, byteorder='big', signed=True)

def byte_array_4_to_int(bytes_array_4):
    """Convert 4 element byte array in big endian order to int_value."""
    return int.from_bytes(bytes_array_4, byteorder='big', signed=True)

def string_to_byte_array(string_value):
    """Convert string_value to byte array."""
    bytes_array = []
    for _, char in enumerate(string_value):
        bytes_array.append(char.encode())

    return bytes_array
