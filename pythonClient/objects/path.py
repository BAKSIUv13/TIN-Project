import tkinter as tk
from data import *


class Path:
    def __init__(self, canvas, r, g, b, a, thick, num, points, size):
        self.points = points
        self.segments = []
        self.r = r
        self.g = g
        self.b = b
        self.a = a
        self.canvas = canvas
        self.thick = thick
        self.num = num
        print([num,points])
        if num > 1:
            for i in range(0, num - 1):
                self.segments.append(
                    self.canvas.create_line(self.points[i*2]*size,
                                            self.points[1 + i*2]*size,
                                            self.points[2 + i*2]*size,
                                            self.points[3 + i*2]*size,
                                            fill="#%02x%02x%02x" % (self.r, self.g, self.b)))

    def set_next(self, x, y):
        self.num += 1
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

    def set_color(self, r, g, b, a):
        self.r = r
        self.g = g
        self.b = b
        self.a = a
        self.canvas.itemconfig(self.oval, fill="#%02x%02x%02x" % (self.r, self.g, self.b)) #TODO dla kazdego segmentu

    def delete(self):
        for item in self.segments:
            self.canvas.delete(item)

    def get_color(self):
        return [self.r, self.g, self.b, self.a]

    def get_params(self, size):
        to_return = self.points
        for i in range(0, self.num*2):
            to_return[i] /= size
        return [self.thick / size, self.num, to_return]