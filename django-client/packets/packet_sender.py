# TIN
# Bartłomiej Kulik

"""Module responsible for sending packets."""

from network_and_data import network, receiver

class PacketSender():
    """Class responsible for sending packets."""

    def __init__(self, sender):
        """Prepare sender."""
        self._sender = sender
        self._sender.start()

    def join_sender_thread(self):
        """Join sender thread."""
        self._sender.join()
