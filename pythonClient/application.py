import tkinter as tk
import time
from data import *


class Application(tk.Frame):
    def __init__(self,parser, master=None):
        self.parser=parser
        super().__init__(master)
        self.master = master
        self.pack()
        self.create_widgets()
        self.refresh()

    def create_widgets(self):
        self.hi_there = tk.Button(self)
        self.hi_there["text"] = "Hello World\n(click me)"
        self.hi_there["command"] = self.say_hi
        self.hi_there.pack(side="top")
        self.text = tk.Text(self)

        self.text.pack()

        self.quit = tk.Button(self, text="QUIT", fg="red",
                              command=self.master.destroy)
        self.quit.pack(side="bottom")

    def say_hi(self):
        print("hi there, everyone!")


    def refresh(self):
        msg: message.Message = self.parser.get_msg()
        if msg.get_type()==2:
            self.text.insert(tk.END, msg.get_name()+": "+msg.get_param()+"\n")
        self.after(100, self.refresh)