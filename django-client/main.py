# TIN
# Bartlomiej Kulik

"""Main program module."""

from network_and_data import network
from network_and_data import receiver

# interface to sockets

SOCKET = network.prepare_client_socket()

RECEIVER = receiver.Receiver(SOCKET)
RECEIVER.start()

RECEIVER.join()



SOCKET.close()

print("the end!")
