import threading
import time

from network import *
from data import message


class Parser(threading.Thread):

    def __init__(self, receiver):
        threading.Thread.__init__(self)
        self.stop_request = threading.Event()
        self.receiver = receiver
        self.msg = message.Message()
        self.msg_is_ready = 0

    def run(self):
        receive = bytearray()
        while not self.stop_request.isSet():
            if not self.msg_is_ready:
                for i in range(0, 40):
                    receive.append(self.receiver.get_byte())
                # print(receive.__sizeof__())
                # print(receive)
                if "OwO!" == receive[0:4].decode():
                    if "maus" == receive[4:8].decode():
                        x = int.from_bytes(receive[8:12], byteorder='big')
                        y = int.from_bytes(receive[12:16], byteorder='big')
                        length = int.from_bytes(receive[16:20], byteorder='big')
                        name = receive[20:20 + length].decode()
                        receive = receive[20 + length:]
                        # print('mause of ' + name + ' x: %d y: %d' % (x, y))
                        param = [x, y]
                        self.msg.set(1, name, param)
                        self.msg_is_ready = 1
                    elif "msg1" == receive[4:8].decode():
                        length = int.from_bytes(receive[8:12], byteorder='big')
                        name = receive[12:12 + length].decode()
                        length1 = int.from_bytes(receive[12 + length:16 + length], byteorder='big')
                        msg = receive[16 + length:16 + length + length1].decode()
                        receive = receive[16 + length + length1:]
                        print('CHAT: ' + name + ': ' + msg)
                        self.msg.set(2, name, msg)
                        self.msg_is_ready = 1
                    else:
                        print("-----------Nieznany komunikat--------------")
            else:
                time.sleep(.100)

    def join(self, timeout=None):
        self.stop_request.set()
        super(Parser, self).join(timeout)
        print("Parser is closed")
        return

    def get_msg(self):
        to_return = message.Message()
        if self.msg_is_ready:
            to_return.set(self.msg.get_type(), self.msg.get_name(), self.msg.get_param())
            self.msg.set(0, 0, 0)
            self.msg_is_ready = 0
            return to_return
        else:
            to_return = message.Message(0, 0, 0)
            return to_return
