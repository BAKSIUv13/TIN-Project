# TIN
# Bartłomiej Kulik

"""Module responsible for receiving data."""

import threading
import queue
import select

# bytes
_RECV_PORTION_SIZE = 4
_RECV_QUEUE_SIZE = 32 * 1024

# seconds
GET_BYTE_TIMEOUT_SEC = 15
RECV_PUT_BYTE_TIMEOUT_SEC = 1

class Receiver(threading.Thread):
    """Class responsible for receiving data."""

    def __init__(self, socket, recv_read_pipe, send_write_pipe):
        """Prepare receiver resources."""
        threading.Thread.__init__(self)
        self._s = socket
        self._r_bytes_queue = queue.Queue(_RECV_QUEUE_SIZE)
        self._recv_read_pipe = recv_read_pipe
        self._send_write_pipe = send_write_pipe

    def run(self):
        """Receive data and check if given pipe is not available."""
        while True:
            avaible_read_sources, _, _ = select.select(
                [self._s, self._recv_read_pipe],
                [],
                [])

            if self._recv_read_pipe in avaible_read_sources:
                # pipe - interrupt
                print('Receiver: pipe - interrupt.')
                self._send_write_pipe.close()
                return
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
                else: # not data
                    # Connection has been lost!
                    print('Receiver: Connection has been lost!')
                    self._send_write_pipe.close()
                    return


    def get_byte(self):
        """
        Return one byte from receiver.

        It blocks at most GET_BYTE_TIMEOUT_SEC and raises the Empty exception
        if no item was available within that time.

        """
        return self._r_bytes_queue.get(block=True,
                                       timeout=GET_BYTE_TIMEOUT_SEC)
