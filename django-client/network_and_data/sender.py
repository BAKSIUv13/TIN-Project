# TIN
# Bartłomiej Kulik

"""Module responsible for sending data."""

import threading
import queue
import select

# bytes
_SEND_PORTION_SIZE = 4
_W_QUEUE_SIZE = 32 * 1024

# seconds
SEND_GET_BYTE_TIMEOUT_SEC = 1
PUT_BYTE_TIMEOUT_SEC = 15

class Sender(threading.Thread):
    """Class responsible for sending data."""

    def __init__(self, socket, send_read_pipe, recv_write_pipe):
        """Prepare sender resources."""
        threading.Thread.__init__(self)
        self._s = socket
        self._w_bytes_queue = queue.Queue(_W_QUEUE_SIZE)
        self._send_read_pipe = send_read_pipe
        self._recv_write_pipe = recv_write_pipe

    def run(self):
        """Send data and check if given pipe is not available."""
        while True:
            avaible_read_sources, avaible_write_sources, _ = select.select(
                [self._send_read_pipe],
                [self._s],
                [])
            if self._send_read_pipe in avaible_read_sources:
                # pipe - interrupt
                print('Sender: pipe - interrupt.')
                self._recv_write_pipe.close()
                return
            if self._s in avaible_write_sources:
                data = []
                for _ in range(_SEND_PORTION_SIZE):
                    try:
                        data.append(self._w_bytes_queue.get(
                            block=True,
                            timeout=SEND_GET_BYTE_TIMEOUT_SEC))
                    except queue.Empty:
                        # empty queue - try send only part of data
                        # maybe _error_read_pipe is available?
                        pass
                if data:
                    for byte in data:
                        try:
                            self._s.sendall(byte)
                        except OSError:
                            # Connection has been lost!
                            print('Sender: Connection has been lost!')
                            self._recv_write_pipe.close()
                            return

    def put_byte(self, byte):
        """Put one byte to sender. Blocks until free space is available."""
        return self._w_bytes_queue.put(byte,
                                       block=True,
                                       timeout=PUT_BYTE_TIMEOUT_SEC)

    def put_byte_array(self, byte_array):
        """Put byte_array to sender. Blocks until free space is available."""
        for byte in byte_array:
            self.put_byte(byte)
