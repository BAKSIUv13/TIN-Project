import threading
import queue

_RECEIVE_PORTION_SIZE = 4
_RECEIVE_QUEUE_SIZE = 32 * 1024

GET_BYTE_TIMEOUT_SEC = 15

class Receiver(threading.Thread):
    """Class responsible for receiving data."""

    def __init__(self, socket):
        """Prepare receiver resources."""
        threading.Thread.__init__(self)
        self._s = socket
        self._r_bytes_queue = queue.Queue(_RECEIVE_QUEUE_SIZE)
        self.stop_request = threading.Event()

    def run(self):
        while not self.stop_request.isSet():
            str = self._s.recv(1024)
            if str:
                for byte in str:
                    # the only place with a put operation
                    try:
                        self._r_bytes_queue.put(
                            byte,
                            block=True,
                            timeout=1)
                    except queue.Full:
                        # full queue - try send again in next loop
                        pass

    def join(self, timeout=None):
        self.stop_request.set()
        super(Receiver, self).join(timeout)
        print("Receiver is closed")
        return

    def get_byte(self):
        return self._r_bytes_queue.get(block=True,
                                       timeout=GET_BYTE_TIMEOUT_SEC)
