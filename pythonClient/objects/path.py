import tkinter as tk
from data import *


class Path:
    def __init__(self, canvas, x, y, r, g, b):
        self.points = [x,y]
        self.segments = []
        self.r = r
        self.g = g
        self.b = b
        self.canvas = canvas

    def set_next(self, x, y):
        self.points.append(x)
        self.points.append(y)
        self.segments.append(
            self.canvas.create_line(self.points[self.points.__len__()-4],
                                    self.points[self.points.__len__()-3],
                                    self.points[self.points.__len__()-2],
                                    self.points[self.points.__len__()-1],
                                    fill="#%02x%02x%02x" % (self.r, self.g, self.b)))

    def set_i(self, x, y, i):
        #TODO
        pass
        self.x2 = x
        self.y2 = y
        self.canvas.coords(self.oval, self.x1, self.y1, x, y)

    def set_color(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b
        self.canvas.itemconfig(self.oval, fill="#%02x%02x%02x" % (self.r, self.g, self.b))

    def delete(self):
        for item in self.segments:
            self.canvas.delete(item)