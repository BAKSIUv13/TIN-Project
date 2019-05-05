# TIN
# Bartłomiej Kulik

"""Module responsible for sending data."""

import threading
import queue
import select

_SEND_PORTION_SIZE = 4 # 4 B
_W_QUEUE_SIZE = 32 * 1024 # 32 kB
SEND_GET_BYTE_TIMEOUT_SEC = 10

class Sender(threading.Thread):
    """Class responsible for sending data."""

    def __init__(self, socket, error_read_pipe):
        """Prepare sender resources."""
        threading.Thread.__init__(self)
        self._s = socket
        self._w_bytes_queue = queue.Queue(_W_QUEUE_SIZE)
        self._error_read_pipe = [error_read_pipe]
    def run(self):
        """Send data and check if given pipe is not available."""
        while True:
            avaible_read_sources, avaible_write_sources, _ = select.select(
                [self._error_read_pipe],
                [self._s],
                [])
            if self._s in avaible_write_sources:
                data = []
                for _ in range(_SEND_PORTION_SIZE):
                    try:
                        data.append(self._w_bytes_queue.get())
                    except queue.Empty:
                        # empty queue - try send only part of data
                        # maybe _error_read_pipe is available?
                        pass

                if data:
                    try:
                        self._s.sendall(data)
                    except Exception:
                        raise OSError('Problem with sending data.')

            if self._error_read_pipe in avaible_read_sources:
                # pipe - interrupt
                self.close_socket()
                raise OSError('Sender have been interrupted.')


    def close_socket(self):
        """Close socket."""
        self._s.close()

    def put_byte(self, byte):
        """Put one byte to sender. Blocks until free space is available."""
        return self._w_bytes_queue.put(byte, block=True, timeout=None)
