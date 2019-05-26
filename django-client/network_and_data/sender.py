# TIN
# Bartłomiej Kulik

"""Module responsible for sending data."""

import queue
import select
import struct
import threading

# bytes objects
_SEND_PORTION_SIZE = 4
_W_QUEUE_SIZE = 1024

# seconds
SEND_GET_BYTE_TIMEOUT_SEC = 1
PUT_BYTE_TIMEOUT_SEC = 15

class Sender(threading.Thread):
    """Class responsible for sending data."""

    def __init__(self, socket, send_read_pipe, recv_write_pipe):
        """Prepare Sender resources."""
        threading.Thread.__init__(self)
        self._s = socket
        self._write_bytes_objects_queue = queue.Queue(_W_QUEUE_SIZE)
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
                        data.append(self._write_bytes_objects_queue.get(
                            block=True,
                            timeout=SEND_GET_BYTE_TIMEOUT_SEC))
                    except queue.Empty:
                        # empty queue - try send only part of data
                        # maybe _error_read_pipe is available?
                        pass
                if data:
                    for byte_object in data:
                        try:
                            self._s.sendall(byte_object)
                        except OSError:
                            # Connection has been lost!
                            print('Sender: Connection has been lost!')
                            self._recv_write_pipe.close()
                            return

    def _put_byte_object(self, byte_object):
        """
        Put one byte_object to sender.

        It blocks at most PUT_BYTE_TIMEOUT_SEC and raises the queue.Empty
        exception if no item was available within that time.
        """
        self._write_bytes_objects_queue.put(byte_object,
                                            block=True,
                                            timeout=PUT_BYTE_TIMEOUT_SEC)

    def _put_byte_object_array(self, byte_object_array):
        """
        Put byte_array to sender.

        It blocks at most PUT_BYTE_TIMEOUT_SEC and raises the queue.Empty
        exception if no item was available within that time.
        """
        for byte in byte_object_array:
            self._put_byte_object(byte)

    def put_string_value(self, string_value):
        """
        Put string_value to sender.

        It blocks at most PUT_BYTE_TIMEOUT_SEC and raises the queue.Empty
        exception if no item was available within that time.
        """
        self._put_byte_object(string_value.encode())

    def put_int32_value(self, int32_value):
        """
        Put int32_value to sender.

        It blocks at most PUT_BYTE_TIMEOUT_SEC and raises the queue.Empty
        exception if no item was available within that time.
        """
        self._put_byte_object(struct.pack('!i', int32_value))

    def put_unsigned_char8_value(self, unsigned_char8_value):
        """
        Put unsigned_char8_value to sender.

        It blocks at most PUT_BYTE_TIMEOUT_SEC and raises the queue.Empty
        exception if no item was available within that time.
        """
        self._put_byte_object(struct.pack('!B', unsigned_char8_value))

    def put_double64_value(self, double64_value):
        """
        Put double_value to sender.

        It blocks at most PUT_BYTE_TIMEOUT_SEC and raises the queue.Empty
        exception if no item was available within that time.
        """
        self._put_byte_object(struct.pack('!d', double64_value))
