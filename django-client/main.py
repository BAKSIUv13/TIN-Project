# TIN
# Bartłomiej Kulik

"""Main program module."""

from network_and_data import network
from network_and_data import receiver
from network_and_data import sender

# interface to sockets

CLIENT_SOCKET = network.prepare_socket()

READ_PIPE, WRITE_PIPE = network.prepare_error_network_pipe()

RECEIVER = receiver.Receiver(CLIENT_SOCKET, READ_PIPE, WRITE_PIPE)
RECEIVER.start()

SENDER = sender.Sender(CLIENT_SOCKET, READ_PIPE)
SENDER.start()

IS_RECV = False

while True:

    if IS_RECV:
        RECV = chr(RECEIVER.get_byte())

        print(RECV)
    else:
        SEND = input()
        if SEND == '0':
            WRITE_PIPE.close()
            break
        for _, CHARACTER in enumerate(SEND):
            SENDER.put_byte(CHARACTER.encode())



RECEIVER.join()
SENDER.join()

CLIENT_SOCKET.close()

print("the end!")
