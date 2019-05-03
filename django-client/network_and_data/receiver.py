# TIN
# Bartlomiej Kulik

"""Module responsible for receive data."""

import threading
import queue
import select
from network_and_data import network

R_QUEUE_ELEMENT_SIZE = 1
R_QUEUE_SIZE = 1024

class Receiver(threading.Thread):
    """Class responsible for receive data."""

    def __init__(self):
        """Prepare socket and other resources."""
        threading.Thread.__init__(self)
        self._s = network.prepare_client_socket(network.HOST, network.PORT)
        self.r_queue = queue.Queue(R_QUEUE_SIZE)
        self._read_sources = [self._s] # + pipe in the future

    def run(self):
        """Receive data and check if is_stopped is true."""
        while True:
            avaible_read_sources, _, _ = select.select(self._read_sources,
                                                       [],
                                                       [])
            for read_source in avaible_read_sources:
                if read_source is self._s:
                    data = self._s.recv(R_QUEUE_ELEMENT_SIZE)
                    if data:
                        self.r_queue.put(data, block=True, timeout=None)
                    else:
                        raise Exception('Connection lost.')
                else:
                    # pipe - stop
                    pass
