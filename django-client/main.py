# TIN
# Bartłomiej Kulik

"""Main program module."""

from network_and_data import network
from network_and_data import receiver
from network_and_data import sender

HOST = 'localhost'
PORT = 12345

# interface to sockets

CLIENT_SOCKET = network.prepare_socket(HOST, PORT)

RECV_READ_PIPE, RECV_WRITE_PIPE = network.prepare_error_pipe()
SEND_READ_PIPE, SEND_WRITE_PIPE = network.prepare_error_pipe()

RECEIVER = receiver.Receiver(CLIENT_SOCKET, RECV_READ_PIPE, SEND_WRITE_PIPE)
SENDER = sender.Sender(CLIENT_SOCKET, SEND_READ_PIPE, RECV_WRITE_PIPE)

RECEIVER.start()
SENDER.start()

POLECENIE = input()

if POLECENIE == 'recv':
    RECV_WRITE_PIPE.write('cos')
    RECV_WRITE_PIPE.close()
else:
    SEND_WRITE_PIPE.close()

RECEIVER.join()
SENDER.join()

CLIENT_SOCKET.close()

print("The end!")
