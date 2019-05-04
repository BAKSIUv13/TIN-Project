# TIN
# Bartlomiej Kulik

"""Module responsible for receiving data."""

import threading
import queue
import select

_RECV_PORTION_SIZE = 4 # 4 B
_R_QUEUE_SIZE = 32 * 1024 # 32 kB
GET_BYTE_TIMEOUT_SEC = 10

class Receiver(threading.Thread):
    """Class responsible for receiving data."""

    def __init__(self, socket, error_read_pipe):
        """Prepare receiver resources."""
        threading.Thread.__init__(self)
        self._s = socket
        self._r_bytes_queue = queue.Queue(_R_QUEUE_SIZE)
        self._read_sources = [self._s, error_read_pipe]
    def run(self):
        """Receive data and check if is_stopped is true."""
        while True:
            avaible_read_sources, _, _ = select.select(self._read_sources,
                                                       [],
                                                       [])
            for read_source in avaible_read_sources:
                if read_source is self._s:
                    data = self._s.recv(_RECV_PORTION_SIZE)
                    if data:
                        for byte in data:
                            # the only place with a put operation
                            self._r_bytes_queue.put(byte,
                                                    block=True,
                                                    timeout=None)
                    else:
                        self.close_socket()
                        raise OSError('Connection lost.')
                else:
                    # pipe - interrupt
                    self.close_socket()
                    raise OSError('Receiver have been interrupted.')

    def close_socket(self):
        """Close socket."""
        self._s.close()

    def get_byte(self):
        """
        Return one byte from receiver.

        It blocks at most GET_BYTE_TIMEOUT_SEC and raises the Empty exception
        if no item was available within that time.

        """
        return self._r_bytes_queue.get(block=True,
                                       timeout=GET_BYTE_TIMEOUT_SEC)
