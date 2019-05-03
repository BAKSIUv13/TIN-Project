# TIN
# Bartlomiej Kulik

"""Main program module."""

from network_and_data import receiver

# interface to sockets


RECEIVER = receiver.Receiver()
RECEIVER.start()

RECEIVER.join()

print("the end!")
