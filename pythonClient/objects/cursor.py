import tkinter as tk
from data import *


class Cursor:
    def __init__(self, canvas, x, y, name):
        self.x = x
        self.y = y
        self.canvas = canvas
        self.name = name
        # self.cursor2=self.canvas.create_rectangle(x-3, y-3, x+3, y+3, fill=color)
        self.obj1 = self.canvas.create_line(x, y - 5, x, y + 5)
        self.obj2 = self.canvas.create_line(x - 5, y, x + 5, y)
        self.obj3 = self.canvas.create_text(x, y + 15, text=name)

    def set(self, x, y):
        self.x = x
        self.y = y
        self.canvas.coords(self.obj1, x, y - 5, x, y + 5)
        self.canvas.coords(self.obj2, x - 5, y, x + 5, y)
        self.canvas.coords(self.obj3, x, y + 10)

    def move_to_the_top(self):
        self.canvas.tag_raise(self.obj1)
        self.canvas.tag_raise(self.obj2)
        self.canvas.tag_raise(self.obj3)

    def get_name(self):
        return self.name
