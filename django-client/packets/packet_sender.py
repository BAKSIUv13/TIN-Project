# TIN
# Bartłomiej Kulik

"""Module responsible for sending packets."""

from network_and_data.sender import Sender
from network_and_data.receiver import Receiver

class PacketSender():
    """Class responsible for sending packets."""

    def __init__(self, sender: Sender):
        """Prepare sender."""
        self._sender = sender
        self._sender.start()

    def send_logo(self, name, password):
        """Send "logo" packet."""
        self._sender.put_string_value("OwO!")
        self._sender.put_string_value("logo")

        self._sender.put_int32_value(len(name))
        self._sender.put_string_value(name)

        self._sender.put_int32_value(len(password))
        self._sender.put_string_value(password)

    def send_lout(self):
        """Send "lout" packet."""
        self._sender.put_string_value("OwO!")
        self._sender.put_string_value("lout")

    def send_mesg(self, message):
        """Send mesg packet."""
        self._sender.put_string_value("OwO!")
        self._sender.put_string_value("mesg")

        self._sender.put_int32_value(len(message))
        self._sender.put_string_value(message)

    def join_sender_thread(self):
        """Join sender thread."""
        self._sender.join()
