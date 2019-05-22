import tkinter as tk
from data import *
from objects import *


class Application(tk.Frame):
    def __init__(self, parser, messenger, master=None):
        self.parser = parser
        self.messenger = messenger
        self.object = 0  # 0-nothing 1-path 2-rectangle 3-oval
        self.drawing = 0  # 0-nothing 1-during drawing process
        self.red = 0
        self.green = 0
        self.blue = 0
        super().__init__(master)
        self.master = master
        self.pack()
        self.create_widgets()
        self.objects = []
        self.refresh()

    def create_widgets(self):

        self.left_panel = tk.Frame(self)
        self.left_panel.pack(anchor="n", side="left", fill="y")

        self.color_panel = tk.Frame(self.left_panel)
        self.color_panel.pack(anchor="n", side="top")

        self.color_label = tk.Label(self.color_panel, text="Colors:")
        self.color_label.pack(anchor="w", side="top")

        self.object_panel = tk.Frame(self.left_panel)
        self.object_panel.pack(anchor="n", side="top")

        self.object_label = tk.Label(self.object_panel, text="Objects:")
        self.object_label.pack(anchor="w", side="top")

        self.r = tk.Entry(self.color_panel)
        self.r.insert(0, "Type your r here")
        self.r.bind('<FocusIn>', self.r_click)
        self.r.bind('<Return>', self.r_leave)
        self.r.pack(anchor="w", side="top")

        self.g = tk.Entry(self.color_panel)
        self.g.insert(0, "Type your g here")
        self.g.bind('<FocusIn>', self.g_click)
        self.g.bind('<Return>', self.g_leave)
        self.g.pack(anchor="w", side="top")

        self.b = tk.Entry(self.color_panel)
        self.b.insert(0, "Type your b here")
        self.b.bind('<FocusIn>', self.b_click)
        self.b.bind('<Return>', self.b_leave)
        self.b.pack(anchor="w", side="top")

        self.path = tk.Button(self.object_panel, text="Path", fg="Black",
                              command=self.path_click)
        self.path.pack(anchor="w", side="left")

        self.rect = tk.Button(self.object_panel, text="Rectangle", fg="Black",
                              command=self.rect_click)
        self.rect.pack(anchor="w", side="left")

        self.oval = tk.Button(self.object_panel, text="Oval", fg="Black",
                              command=self.oval_click)
        self.oval.pack(anchor="w", side="left")

        self.list_of_objects = tk.Listbox(self.left_panel, width=20, height=10)
        self.list_of_objects.bind("<<ListboxSelect>>", self.choose_object)
        self.list_of_objects.pack(anchor="w", side="top")

        self.edit_panel = tk.Frame(self.left_panel)
        self.edit_panel.pack(anchor="n", side="top")

        self.edit = tk.Button(self.edit_panel, text="Edit", fg="Black",
                                     command=self.edit_object)

        self.delete = tk.Button(self.edit_panel, text="Delete", fg="Black",
                                       command=self.delete_object)

        self.canvas = tk.Canvas(self, bg="white", width=1000, height=500);
        self.canvas.pack(side="top")
        self.canvas.bind('<1>', self.canvas_click)

        self.cursor2 = cursor.Cursor(self.canvas, 10, 10, "baksiu")

        self.text_frame = tk.Frame(self)
        self.text_frame.pack()

        self.text = tk.Text(self.text_frame, width=100, height=10)
        self.text.pack(side="left")

        self.scroll_bar = tk.Scrollbar(self.text_frame, command=self.text.yview)
        self.scroll_bar.pack(side="right", fill="y")
        self.text['yscrollcommand'] = self.scroll_bar.set

        self.field = tk.Entry(self)
        self.field.insert(0, "Type your text here")
        self.field.bind('<FocusIn>', self.field_click)
        self.field.bind('<Return>', self.field_leave)
        self.field.pack(side="top")

        self.send = tk.Button(self, text="SEND", fg="Black",
                              command=self.send)
        self.send.pack(side="top")

        self.quit = tk.Button(self, text="QUIT", fg="red",
                              command=self.master.destroy)
        self.quit.pack(side="bottom")

    def refresh(self):
        msg: message.Message = self.parser.get_msg()
        if msg.get_type() == 2:
            self.text.insert(tk.END, msg.get_name() + ": " + msg.get_param() + "\n")
            if self.scroll_bar.get()[1] == 1.0:
                self.text.see(tk.END)
        if msg.get_type() == 1:
            if msg.get_name() == "baksiubaksiu":
                self.cursor2.set(msg.get_param()[0], msg.get_param()[1])
        self.after(100, self.refresh)

    def send(self):
        if not self.field.get() == "Type your text here" and not self.field.get() == "":
            str = self.field.get()
            self.field.delete(0, "end")
            msg = message.Message(2, "nazwa_moja", str)
            self.messenger.put_msg(msg)

    def field_click(self, event):
        if self.field.get() == "Type your text here":
            self.field.delete(0, "end")
            self.field.insert(0, "")

    def field_leave(self, event):
        if self.field.get() == "":
            self.field.insert(0, "Type your text here")

    def r_click(self, event):
        if self.r.get() == "Type your r here" or self.r.get() == "Type value 0-255":
            self.r.delete(0, "end")
            self.r.insert(0, str(self.red))

    def r_leave(self, event):
        try:
            if self.r.getint(self.r.get()) < 0 or self.r.getint(self.r.get()) > 255:
                self.r.delete(0, "end")
                self.r.insert(0, "Type value 0-255")
            if self.r.get() == "":
                self.r.insert(0, "Type your r here")
            else:
                self.red = self.r.getint(self.r.get())
        except:
            self.b.delete(0, "end")
            self.b.insert(0, "Type value 0-255")

    def g_click(self, event):
        if self.g.get() == "Type your g here" or self.g.get() == "Type value 0-255":
            self.g.delete(0, "end")
            self.g.insert(0, str(self.green))

    def g_leave(self, event):
        try:
            if self.g.getint(self.g.get()) < 0 or self.g.getint(self.g.get()) > 255:
                self.g.delete(0, "end")
                self.g.insert(0, "Type value 0-255")
            if self.g.get() == "":
                self.g.insert(0, "Type your g here")
            else:
                self.green = self.g.getint(self.g.get())
        except:
            self.b.delete(0, "end")
            self.b.insert(0, "Type value 0-255")

    def b_click(self, event):
        if self.b.get() == "Type your b here" or self.b.get() == "Type value 0-255":
            self.b.delete(0, "end")
            self.b.insert(0, str(self.blue))

    def b_leave(self, event):
        try:
            if self.b.getint(self.b.get()) < 0 or self.b.getint(self.b.get()) > 255:
                self.b.delete(0, "end")
                self.b.insert(0, "Type value 0-255")
            if self.b.get() == "":
                self.b.insert(0, "Type your b here")
            else:
                self.blue = self.b.getint(self.b.get())
        except:
            self.b.delete(0, "end")
            self.b.insert(0, "Type value 0-255")

    def canvas_click(self, event):
        print(str(event.x) + str(event.y))
        if self.object == 1:
            if self.drawing == 0:
                self.objects.append(path.Path(self.canvas, event.x, event.y, self.red, self.green, self.blue))
                self.list_of_objects.insert(tk.END, self.objects[self.objects.__len__() - 1])
                self.cursors_on_top()
                self.drawing = 1
            else:
                self.objects[self.objects.__len__() - 1].set_next(event.x, event.y)
                self.cursors_on_top()
        elif self.object == 2:
            if self.drawing == 0:
                self.objects.append(rectangle.Rectangle(self.canvas, event.x, event.y, self.red, self.green, self.blue))
                self.list_of_objects.insert(tk.END, self.objects[self.objects.__len__() - 1])
                self.cursors_on_top()
                self.drawing = 1
            else:
                self.objects[self.objects.__len__() - 1].set2(event.x, event.y)
                self.cursors_on_top()
                self.drawing = 0
                self.object = 0
        elif self.object == 3:
            if self.drawing == 0:
                self.objects.append(oval.Oval(self.canvas, event.x, event.y, self.red, self.green, self.blue))
                self.list_of_objects.insert(tk.END, self.objects[self.objects.__len__() - 1])
                self.cursors_on_top()
                self.drawing = 1
            else:
                self.objects[self.objects.__len__() - 1].set2(event.x, event.y)
                self.cursors_on_top()
                self.drawing = 0
                self.object = 0

    def path_click(self):
        if self.object == 1 and self.drawing == 1:
            self.drawing = 0
            self.object = 0
        elif self.drawing == 0:
            self.object = 1
        else:
            self.object = 0


    def rect_click(self):
        if self.drawing == 0:
            self.object = 2

    def oval_click(self):
        if self.drawing == 0:
            self.object = 3

    def cursors_on_top(self):
        # TODO lista kursoruf
        self.cursor2.move_to_the_top()

    def choose_object(self, event):
        if not self.list_of_objects.curselection() == ():
            self.edit.pack(anchor="w", side="left")
            self.delete.pack(anchor="w", side="left")

    def edit_object(self):
        #TODO
        self.edit.pack_forget()
        self.delete.pack_forget()

    def delete_object(self):
        num, = self.list_of_objects.curselection()
        self.list_of_objects.delete(num, num)
        self.objects[num].delete()
        self.objects.remove(self.objects[num])
        self.edit.pack_forget()
        self.delete.pack_forget()