# TIN
# Bartłomiej Kulik

"""Main program module."""

from network_and_data import network
from network_and_data import receiver
from network_and_data import sender

HOST = 'localhost'
PORT = 22345

# interface to sockets

CLIENT_SOCKET = network.prepare_socket(HOST, PORT)

RECV_READ_PIPE, RECV_WRITE_PIPE = network.prepare_error_pipe()
SEND_READ_PIPE, SEND_WRITE_PIPE = network.prepare_error_pipe()

RECEIVER = receiver.Receiver(CLIENT_SOCKET, RECV_READ_PIPE, SEND_WRITE_PIPE)
SENDER = sender.Sender(CLIENT_SOCKET, SEND_READ_PIPE, RECV_WRITE_PIPE)

RECEIVER.start()
SENDER.start()

"""

SENDER.put_byte_array(network.string_to_byte_array('OwO!sesskkkkkkkk'))
SENDER.put_byte_array(network.string_to_byte_array('OwO!msg0'))
SENDER.put_byte_array(network.string_to_byte_array('1234'))
SENDER.put_byte_array(network.string_to_byte_array('tekstwiadotekstwiadotekstwiado'))


ARRAY = RECEIVER.get_byte_array(8 + 8 + 16 + 4 + 30)

for byte in ARRAY:
    print(chr(byte))

"""

"""
ARRAY = network.int_to_byte_array_4(13)

for byte in ARRAY:
    string = chr(byte)
    byte_array = network.string_to_byte_array(string)
    SENDER.put_byte_array(byte_array)

"""

RECEIVER.join()
SENDER.join()

CLIENT_SOCKET.close()

print("The end!")
