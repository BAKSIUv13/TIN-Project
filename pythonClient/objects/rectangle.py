import tkinter as tk
from data import *


class Rectangle:
    def __init__(self, canvas, x, y, r, g, b):
        self.x1 = x
        self.y1 = y
        self.x2 = x
        self.y2 = y
        self.r = r
        self.g = g
        self.b = b
        self.canvas = canvas
        self.rect = self.canvas.create_rectangle(x, y, x, y, fill="#%02x%02x%02x" % (self.r, self.g, self.b))  # TODO
        # self.rect=self.canvas.create_rectangle(x, y, x, y, fill='', outline="green")

    def set1(self, x, y):
        self.x1 = x
        self.y1 = y
        self.canvas.coords(self.rect, x, y, self.x2, self.y2)

    def set2(self, x, y):
        self.x2 = x
        self.y2 = y
        self.canvas.coords(self.rect, self.x1, self.y1, x, y)

    def set_color(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b
        self.canvas.itemconfig(self.rect, fill="#%02x%02x%02x" % (self.r, self.g, self.b))
