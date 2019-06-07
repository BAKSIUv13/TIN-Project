import threading
import time
import struct

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
                for i in range(0, 400):
                    try:
                        receive.append(self.receiver.get_byte())
                    except:
                        pass
                    # a=self.receiver.get_byte()
                    # if a:
                    #    receive += a.to_bytes(4,byteorder='big')
                    # print(receive)
                # print(receive.__sizeof__()):
                # print(receive)
                try:
                    if "OwO!" == receive[0:4].decode():
                        if "LGOK" == receive[4:8].decode():
                            print("Pomyslnioe zalogowano")
                            receive = receive[8:]
                        elif "MESG" == receive[4:8].decode():
                            print("Masz wiadomosc")
                            len = self.get_int32(receive[8:12])
                            print("od " + receive[12:12 + len].decode())
                            len1 = self.get_int32(receive[12 + len:16 + len])
                            print(": " + receive[16 + len:16 + len + len1].decode())
                            self.msg.set(2, receive[12:12 + len].decode(), receive[16 + len:16 + len + len1].decode())
                            self.msg_is_ready = 1
                            receive = receive[16 + len + len1:]
                        elif "MAUS" == receive[4:8].decode():
                            x = self.get_double64(receive[8:16])
                            y = self.get_double64(receive[16:24])
                            length = int.from_bytes(receive[24:28], byteorder='big')
                            name = receive[28:28 + length].decode()
                            receive = receive[28 + length:]
                            # print('mause of ' + name + ' x: %d y: %d' % (x, y))
                            param = [x, y]
                            self.msg.set(1, name, param)
                            self.msg_is_ready = 1
                        elif "NEWW" == receive[4:8].decode():
                            id = self.get_int32(receive[8:12])
                            username_len = self.get_int32(receive[12:16])
                            name = receive[16:16 + username_len].decode()
                            shape = receive[16 + username_len:20 + username_len].decode()
                            if shape == "rect":
                                [r1, g1, b1, a1] = [self.get_char8(receive[20 + username_len:21 + username_len]),
                                                    self.get_char8(receive[21 + username_len:22 + username_len]),
                                                    self.get_char8(receive[22 + username_len:23 + username_len]),
                                                    self.get_char8(receive[23 + username_len:24 + username_len])]
                                [r2, g2, b2, a2] = [self.get_char8(receive[24 + username_len:25 + username_len]),
                                                    self.get_char8(receive[25 + username_len:26 + username_len]),
                                                    self.get_char8(receive[26 + username_len:27 + username_len]),
                                                    self.get_char8(receive[27 + username_len:28 + username_len])]
                                thick = self.get_double64(receive[28 + username_len:36 + username_len])
                                x = self.get_double64(receive[36 + username_len:44 + username_len])
                                y = self.get_double64(receive[44 + username_len:52 + username_len])
                                xx = self.get_double64(receive[52 + username_len:60 + username_len])
                                yy = self.get_double64(receive[60 + username_len:68 + username_len])
                                receive = receive[68 + username_len:]
                                self.msg.set_shape("rect", [r1, g1, b1, a1], [r2, g2, b2, a2], [thick, x, y, xx, yy], 0,
                                                   0)
                                self.msg_is_ready = 1
                            elif shape == "oval":
                                [r1, g1, b1, a1] = [self.get_char8(receive[20 + username_len:21 + username_len]),
                                                    self.get_char8(receive[21 + username_len:22 + username_len]),
                                                    self.get_char8(receive[22 + username_len:23 + username_len]),
                                                    self.get_char8(receive[23 + username_len:24 + username_len])]
                                [r2, g2, b2, a2] = [self.get_char8(receive[24 + username_len:25 + username_len]),
                                                    self.get_char8(receive[25 + username_len:26 + username_len]),
                                                    self.get_char8(receive[26 + username_len:27 + username_len]),
                                                    self.get_char8(receive[27 + username_len:28 + username_len])]
                                thick = self.get_double64(receive[28 + username_len:36 + username_len])
                                x = self.get_double64(receive[36 + username_len:44 + username_len])
                                y = self.get_double64(receive[44 + username_len:52 + username_len])
                                xx = self.get_double64(receive[52 + username_len:60 + username_len])
                                yy = self.get_double64(receive[60 + username_len:68 + username_len])
                                receive = receive[68 + username_len:]
                                print([thick, x, y, xx, yy])
                                self.msg.set_shape("oval", [r1, g1, b1, a1], [r2, g2, b2, a2], [thick, x, y, xx, yy], 0,
                                                   0)
                                self.msg_is_ready = 1
                            elif shape == "path":
                                [r1, g1, b1, a1] = [self.get_char8(receive[20 + username_len:21 + username_len]),
                                                    self.get_char8(receive[21 + username_len:22 + username_len]),
                                                    self.get_char8(receive[22 + username_len:23 + username_len]),
                                                    self.get_char8(receive[23 + username_len:24 + username_len])]
                                thick = self.get_double64(receive[24 + username_len:32 + username_len])
                                num = self.get_int32(receive[32 + username_len:36 + username_len])
                                points = []
                                for i in range(0, num):
                                    points.append(
                                        self.get_double64(receive[36 + username_len + i * 16:44 + username_len + i * 16]))
                                    points.append(
                                        self.get_double64(receive[44 + username_len + i * 16:52 + username_len + i * 16]))
                                receive = receive[36 + username_len + num * 16:]
                                self.msg.set_path("path", [r1, g1, b1, a1], [thick, num, points])
                                self.msg_is_ready = 1
                            else:
                                receive = receive[20 + username_len:]
                        elif "LSSH" == receive[4:8].decode():
                            num = self.get_int32(receive[8:12])
                            print("jest " + str(num) + " figurek")
                            receive = receive[12:]
                            print(receive)
                            for i in range(0, num):
                                print(receive)  # TODO
                                id = self.get_int32(receive[0:4])
                                if "rect" == receive[4:8].decode():
                                    print("rect")
                                    [r1, g1, b1, a1] = [self.get_char8(receive[8:9]),
                                                        self.get_char8(receive[9:10]),
                                                        self.get_char8(receive[10:11]),
                                                        self.get_char8(receive[11:12])]
                                    [r2, g2, b2, a2] = [self.get_char8(receive[12:13]),
                                                        self.get_char8(receive[13:14]),
                                                        self.get_char8(receive[14:15]),
                                                        self.get_char8(receive[15:16])]
                                    thick = self.get_double64(receive[16:24])
                                    x = self.get_double64(receive[24:32])
                                    y = self.get_double64(receive[32:40])
                                    xx = self.get_double64(receive[40:48])
                                    yy = self.get_double64(receive[48:56])
                                    receive = receive[56:]
                                    print("rect")
                                    self.msg.set_shape("rect", [r1, g1, b1, a1], [r2, g2, b2, a2],
                                                       [thick, x, y, xx, yy], 0)
                                    self.msg_is_ready = 1
                                elif "oval" == receive[0:4].decode():
                                    print("oval")
                                    [r1, g1, b1, a1] = [self.get_char8(receive[8:9]),
                                                        self.get_char8(receive[9:10]),
                                                        self.get_char8(receive[10:11]),
                                                        self.get_char8(receive[11:12])]
                                    [r2, g2, b2, a2] = [self.get_char8(receive[12:13]),
                                                        self.get_char8(receive[13:14]),
                                                        self.get_char8(receive[14:15]),
                                                        self.get_char8(receive[15:16])]
                                    thick = self.get_double64(receive[16:24])
                                    x = self.get_double64(receive[24:32])
                                    y = self.get_double64(receive[32:40])
                                    xx = self.get_double64(receive[40:48])
                                    yy = self.get_double64(receive[48:56])
                                    receive = receive[56:]
                                    print("oval")
                                else:
                                    receive = receive[4:]
                        else:
                            print("-----------Nieznany komunikat--------------")
                            print(receive)
                            receive = receive[4:]
                    else:
                        if receive.__len__() > 4:
                            receive = receive[1:]
                except:
                    # print("Cos tak nie : ")
                    receive = receive[1:]
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
            to_return.clone(self.msg)
            self.msg.set(0, 0, 0)
            self.msg_is_ready = 0
            return to_return
        else:
            to_return = message.Message(0, 0, 0)
            return to_return

    def get_int32(self, encoded_int32):
        return struct.unpack('!i', encoded_int32)[0]

    def get_double64(self, encoded_double64):
        return struct.unpack('!d', encoded_double64)[0]

    def get_char8(self, encoded_char8):
        return struct.unpack('!B', encoded_char8)[0]
