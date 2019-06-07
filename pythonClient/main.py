import time
import tkinter as tk
import struct

from application import Application
from login import Login
from network import *
from data import *

HOST = '127.0.0.1'
PORT = 12345
#HOST = '89.65.50.10'
#PORT = 59898
NAME = ""


def set(name, host, port):
    global HOST
    global PORT
    global NAME
    HOST = host
    PORT = port
    NAME = name


# TODO autoryzacja danych wejsciowych
"""while True:
    login_screen = tk.Tk()
    login_screen.geometry("600x200")
    login = Login(set,master=login_screen)
    login.mainloop()
    break"""

client_socket = network.prepare_socket(HOST, PORT)

receiver = receiver.Receiver(client_socket)
sender = sender.Sender(client_socket)
parser = parser.Parser(receiver)
messenger = messenger.Messenger(sender)

receiver.start()
sender.start()
parser.start()
messenger.start()

# str1="OwO!logo\x00\x00\x00\x05aaaaa\x00\x00\x00\x02aa"
# client_socket.send(str1.encode())
sender.put_string("OwO!logo")
sender.put_int32(5)
sender.put_string("RF Y8")
sender.put_int32(6)
sender.put_string("admin1")

sender.put_string("OwO!mesg")
sender.put_int32(10)
sender.put_string("0123456789")

"""sender.put_string("OwO!creaoval")
sender.put_char8(255)
sender.put_char8(255)
sender.put_char8(255)
sender.put_char8(255)
sender.put_char8(255)
sender.put_char8(255)
sender.put_char8(255)
sender.put_char8(255)
sender.put_double64(0.1)
sender.put_double64(0.1)
sender.put_double64(0.1)
sender.put_double64(0.4)
sender.put_double64(0.2)"""
"""sender.put_string("OwO!creapath")
sender.put_char8(255)
sender.put_char8(255)
sender.put_char8(255)
sender.put_char8(255)
sender.put_double64(0.1)
sender.put_int32(3)
sender.put_double64(0.1)
sender.put_double64(0.3)
sender.put_double64(0.3)
sender.put_double64(0.2)
sender.put_double64(0.2)
sender.put_double64(0.5)"""
sender.put_string("OwO!lssh")
sender.put_string("OwO!lsus")

root = tk.Tk()
root.geometry("1200x800")
app = Application(parser, messenger, master=root)
app.mainloop()

sender.put_string("OwO!lout")

messenger.join()
parser.join()
receiver.join()
sender.join()

client_socket.close()
