import threading
import time

from network import *
from data import message


class Messenger(threading.Thread):

    def __init__(self, sender):
        threading.Thread.__init__(self)
        self.to_send = ""
        self.stop_request = threading.Event()
        self.sender = sender
        self.msg_is_ready = 0

    def run(self):
        while not self.stop_request.isSet():
            while not self.msg_is_ready == 1 and not self.stop_request.isSet():
                time.sleep(.100)
            message = self.to_send.encode('ascii')
            self.to_send = ""
            for byte in message:
                self.sender.put_byte(byte)
            self.msg_is_ready = 0

    def join(self, timeout=None):
        self.stop_request.set()
        super(Messenger, self).join(timeout)
        print("Messenger is closed")
        return

    def put_msg(self, msg):
        while not self.msg_is_ready == 0 and not self.stop_request.isSet():
            time.sleep(.100)
        self.to_send = []
        if msg.get_type() == 2:
            self.to_send.append("msg0")
            self.to_send.append(msg.get_name())
            self.to_send.append(msg.get_param())
        self.to_send = str(self.to_send)
        self.msg_is_ready = 1
        print(self.to_send)
