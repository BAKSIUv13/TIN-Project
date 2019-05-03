# TIN
# Bartlomiej Kulik

"""Module responsible for receive data."""

import threading
import queue
import select
from network_and_data import network

R_QUEUE_ELEMENT_SIZE = 4 # bytes
R_QUEUE_SIZE = 1024 # 1024 * 4 = 4096 bytes
SELECT_TIMEOUT = 10.0

class Receiver(threading.Thread):
    """Class responsible for receive data."""

    def __init__(self):
        """Prepare socket, r_queue and flags."""
        threading.Thread.__init__(self)
        self._s = network.prepare_socket(network.HOST, network.PORT)
        self.r_queue = queue.Queue(R_QUEUE_SIZE)
        self._read_channel = [self._s]

    def run(self):
        """Receive data and check if is_stopped is true."""
        while True:
            to_read, _, _ = select.select(self._read_channel, [], [], )
            for read in to_read:
                if read is self._s:
                    data = self._s.recv(R_QUEUE_ELEMENT_SIZE)
                    if data:
                        self.r_queue.put(data, block=True, timeout=None)
