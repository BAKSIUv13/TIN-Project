# TIN
# Bartłomiej Kulik

"""Module responsible for receiving data."""

import threading
import queue
import select

_RECV_PORTION_SIZE = 4 # 4 B
_R_QUEUE_SIZE = 32 * 1024 # 32 kB
GET_BYTE_TIMEOUT_SEC = 60
RECV_PUT_BYTE_TIMEOUT_SEC = 1

class Receiver(threading.Thread):
    """Class responsible for receiving data."""

    def __init__(self, socket, error_read_pipe, error_write_pipe):
        """Prepare receiver resources."""
        threading.Thread.__init__(self)
        self._s = socket
        self._r_bytes_queue = queue.Queue(_R_QUEUE_SIZE)
        self._error_read_pipe = error_read_pipe
        self._error_write_pipe = error_write_pipe
    def run(self):
        """Receive data and check if given pipe is not available."""
        while True:
            avaible_read_sources, _, _ = select.select(
                [self._s, self._error_read_pipe],
                [],
                [])

            if self._s in avaible_read_sources:
                data = self._s.recv(_RECV_PORTION_SIZE)
                if data:
                    for byte in data:
                        # the only place with a put operation
                        try:
                            self._r_bytes_queue.put(
                                byte,
                                block=True,
                                timeout=RECV_PUT_BYTE_TIMEOUT_SEC)
                        except queue.Full:
                            # full queue - try send again in next loop
                            # maybe _error_read_pipe is available?
                            pass

                else:
                    # Connection has been lost!
                    # TODO
                    print('Receiver: Connection has been lost!')
                    self._error_write_pipe.write('Receiver interrupt...')
                    self._error_write_pipe.close()
                    return
                if self._error_read_pipe in avaible_read_sources:
                    # pipe - interrupt
                    # TODO
                    print('Receiver: pipe - interrupt')
                    self._error_write_pipe.write('Receiver interrupt...')
                    self._error_write_pipe.close()
                    return

    def get_byte(self):
        """
        Return one byte from receiver.

        It blocks at most GET_BYTE_TIMEOUT_SEC and raises the Empty exception
        if no item was available within that time.

        """
        return self._r_bytes_queue.get(block=True,
                                       timeout=GET_BYTE_TIMEOUT_SEC)
