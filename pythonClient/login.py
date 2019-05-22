import tkinter as tk
from data import *


class Login(tk.Frame):
    def __init__(self, set, master=None):
        super().__init__(master)
        self.set = set
        self.master = master
        self.pack()
        self.create_widgets()

    def create_widgets(self):
        self.name = tk.Entry(self)
        self.name.insert(0, "Type your name here")
        self.name.bind('<FocusIn>', self.on_name_click)
        self.name.bind('<FocusOut>', self.on_name_leave)
        self.name.pack(side="top")

        self.host = tk.Entry(self)
        self.host.insert(0, "127.0.0.1")
        self.host.pack(side="top")

        self.port = tk.Entry(self)
        self.port.insert(0, "12345")
        self.port.pack(side="top")

        self.send = tk.Button(self, text="LOG IN", fg="Black",
                              command=self.send)
        self.send.pack(side="top")

        self.quit = tk.Button(self, text="QUIT", fg="red",
                              command=self.master.destroy)
        self.quit.pack(side="bottom")

    def send(self):
        name = self.name.get()
        host = self.host.get()
        port = self.port.get()
        self.set(name, host, port)

    def on_name_click(self, event):
        if self.name.get() == "Type your name here":
            self.name.delete(0, "end")
            self.name.insert(0, "")

    def on_name_leave(self, event):
        if self.name.get() == "":
            self.name.insert(0, "Type your name here")
