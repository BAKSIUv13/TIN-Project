# TIN
# Bartłomiej Kulik

# TIN
# Bartłomiej Kulik

"""Module responsible for receiving packets."""

MESSAGE_MAX_SIZE = 8000
USERNAME_MAX_SIZE = 40

from network_and_data.sender import Sender
from network_and_data.receiver import Receiver

class PacketReceiver():
    """Class responsible for receiving packets."""

    def __init__(self, receiver: Receiver):
        """Prepare sender."""
        self._receiver = receiver
        self._receiver.start()

    def receive_packet(self):
        """Method responsible for receiving packets."""
        header = self._receiver.get_string_value(4)
        if header != 'OwO!':
            return self._bad_packet()

        # 'OwO!' header

        packet_id = self._receiver.get_string_value(4)

        if packet_id == 'LGOK':
            return self._lgok()

        if packet_id == 'SIGL':
            code = self._receiver.get_string_value(4)
            len_message = self._receiver.get_int32_value()
            message = None
            if len_message < MESSAGE_MAX_SIZE:
                message = self._receiver.get_string_value(len_message)

            return self._sigl(code, message)

        if packet_id == 'LOFF':
            return self._loff

        if packet_id == 'MESG':
            len_username = self._receiver.get_int32_value()
            username = None
            if len_username < USERNAME_MAX_SIZE:
                username = self._receiver.get_string_value(len_username)
            len_message = self._receiver.get_int32_value()
            message = None
            if len_message < MESSAGE_MAX_SIZE:
                message = self._receiver.get_string_value(len_message)

            return self._message(username, message)


    def _bad_packet(self):
        print('bad packet')

    def _lgok(self):
        print('lgok')
        return True

    def _loff(self):
        print('loff')
        return True

    def _sigl(self, code, message):
        print(code, message)

    def _message(self, username, message):
        print(username + ': ' + message)


    def join_receiver_thread(self):
        """Join receiveer thread."""
        self._receiver.join()
