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
messenger = messenger.Messenger(sender)

receiver.start()
sender.start()
parser.start()
messenger.start()

client_socket.send('ip login haslo'.encode())
client_socket.send('yyyyyyyyyyyyyyyyyy'.encode())

root = tk.Tk()
root.geometry("1200x800")
app = Application(parser, messenger, master=root)
app.mainloop()

messenger.join()
parser.join()
receiver.join()
sender.join()

client_socket.close()
