# TIN
# Bartłomiej Kulik

"""Main program module."""

import queue

from network_and_data import network
from network_and_data import receiver

# interface to sockets

CLIENT_SOCKET = network.prepare_socket()

READ_PIPE, WRITE_PIPE = network.prepare_error_network_pipe()

RECEIVER = receiver.Receiver(CLIENT_SOCKET, READ_PIPE)
RECEIVER.start()

while True:
    try:
        BYTE = RECEIVER.get_byte()
    except queue.Empty:
        print('Empty :D')
        break
    if BYTE == 48:
        WRITE_PIPE.write('dupa')
        WRITE_PIPE.close()
    else:
        print(BYTE)


RECEIVER.join()
CLIENT_SOCKET.close()

print("the end!")
