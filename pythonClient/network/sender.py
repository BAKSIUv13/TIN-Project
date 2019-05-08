import threading
import queue

_SEND_PORTION_SIZE = 4
_SEND_QUEUE_SIZE = 32 * 1024

PUT_BYTE_TIMEOUT_SEC = 15
SEND_GET_BYTE_TIMEOUT_SEC=1


class Sender(threading.Thread):
    """Class responsible for sending data."""

    def __init__(self, socket):
        threading.Thread.__init__(self)
        self._s = socket
        self._w_bytes_queue = queue.Queue(_SEND_QUEUE_SIZE)
        self.stop_request = threading.Event()

    def run(self):
        while not self.stop_request.isSet():
            data = []
            for _ in range(_SEND_PORTION_SIZE):
                try:
                    data.append(self._w_bytes_queue.get(
                        block=True,
                        timeout=SEND_GET_BYTE_TIMEOUT_SEC))
                except queue.Empty:
                    # empty queue - try send only part of data
                    pass
            if data:
                for byte in data:
                    self._s.sendall(byte.to_bytes(4,byteorder='big'))

    def join(self, timeout=None):
        self.stop_request.set()
        super(Sender, self).join(timeout)
        print("Sender is closed")
        return

    def put_byte(self, byte):
        return self._w_bytes_queue.put(byte,
                                       block=True,
                                       timeout=PUT_BYTE_TIMEOUT_SEC)