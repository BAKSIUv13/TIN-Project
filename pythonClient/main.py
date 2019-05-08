import time
import tkinter as tk

from application import Application
from network import *
from data import *

HOST = '127.0.0.1'
PORT = 12345

client_socket = network.prepare_socket(HOST, PORT)

receiver = receiver.Receiver(client_socket)
sender = sender.Sender(client_socket)
parser = parser.Parser(receiver)

receiver.start()
sender.start()
parser.start()

root = tk.Tk()
root.geometry("800x600")
app = Application(parser, master=root)
app.mainloop()

client_socket.send('ip login haslo'.encode())

time.sleep(10)

parser.join()
receiver.join()
sender.join()

client_socket.close()
