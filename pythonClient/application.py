import tkinter as tk
import time
from data import *


class Application(tk.Frame):
    def __init__(self,parser, messenger, master=None):
        self.parser=parser
        self.messenger=messenger
        super().__init__(master)
        self.master = master
        self.pack()
        self.create_widgets()
        self.refresh()

    def create_widgets(self):
        self.canvas = tk.Canvas(self,bg="white",width=1000,height=500);
        self.canvas.pack(side="top")
        self.cursor1=self.canvas.create_rectangle(5, 5, 10, 10, fill="blue")

        self.text = tk.Text(self,width=100,height=10)
        self.text.pack()

        self.field=tk.Entry(self)
        self.field.insert(0, "Type your text here")
        self.field.pack(side="top")

        self.send = tk.Button(self, text="SEND", fg="Black",
                              command=self.send)
        self.send.pack(side="top")

        self.quit = tk.Button(self, text="QUIT", fg="red",
                              command=self.master.destroy)
        self.quit.pack(side="bottom")

    def say_hi(self):
        print("hi there, everyone!")


    def refresh(self):
        msg: message.Message = self.parser.get_msg()
        if msg.get_type()==2:
            self.text.insert(tk.END, msg.get_name()+": "+msg.get_param()+"\n")
        if msg.get_type()==1:
            self.canvas.coords(self.cursor1,msg.get_param()[0]-3,msg.get_param()[1]-3,msg.get_param()[0]+3,msg.get_param()[1]+3)
        self.after(100, self.refresh)

    def send(self):
        str=self.field.get()
        msg=message.Message(2,"nazwa_moja",str)
        self.messenger.put_msg(msg)
