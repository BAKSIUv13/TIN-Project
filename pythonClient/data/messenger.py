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
        self.msg = message.Message()

    def run(self):
        while not self.stop_request.isSet():
            while not self.msg_is_ready == 1 and not self.stop_request.isSet():
                time.sleep(.100)
            #message = self.to_send.encode('ascii')
            #self.to_send = ""
            #for byte in message:
            #    self.sender.put_byte(byte.to_bytes(1,byteorder='big'))
            if self.msg.get_type() == 2:
                self.sender.put_string("OwO!mesg")
                self.sender.put_int32(self.msg.get_param().__len__())
                self.sender.put_string(self.msg.get_param())
            elif self.msg.get_type() == 3:
                if self.msg.get_shape_type() == "rect":
                    self.sender.put_string("OwO!crearect")
                    self.sender.put_char8(self.msg.get_shape()[0][0])
                    self.sender.put_char8(self.msg.get_shape()[0][1])
                    self.sender.put_char8(self.msg.get_shape()[0][2])
                    self.sender.put_char8(self.msg.get_shape()[0][3])
                    self.sender.put_char8(self.msg.get_shape()[1][0])
                    self.sender.put_char8(self.msg.get_shape()[1][1])
                    self.sender.put_char8(self.msg.get_shape()[1][2])
                    self.sender.put_char8(self.msg.get_shape()[1][3])
                    self.sender.put_double64(self.msg.get_shape()[2][0])
                    self.sender.put_double64(self.msg.get_shape()[2][1])
                    self.sender.put_double64(self.msg.get_shape()[2][2])
                    self.sender.put_double64(self.msg.get_shape()[2][3])
                    self.sender.put_double64(self.msg.get_shape()[2][4])
            self.msg_is_ready = 0

    def join(self, timeout=None):
        self.stop_request.set()
        super(Messenger, self).join(timeout)
        print("Messenger is closed")
        return

    def put_msg(self, msg):
        while not self.msg_is_ready == 0 and not self.stop_request.isSet():
            time.sleep(.100)
        self.msg=msg
        #self.to_send = []
        #if msg.get_type() == 2:
        #    self.to_send.append("msg0")
        #    self.to_send.append(msg.get_name())
        #    self.to_send.append(msg.get_param())
        #self.to_send = str(self.to_send)
        self.msg_is_ready = 1
        print(self.to_send)
