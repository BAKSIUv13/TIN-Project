class Message():

    def __init__(self, type=0, name="", param=""):
        self.type = type
        # 0 - nothing
        # 1 - position of maus
        # 2 - msg
        # 3 - shape
        self.name = name
        self.param = param
        self.rgba = [0, 0, 0, 0]
        self.rgba2 = [0, 0, 0, 0]
        self.shape = ""
        self.shape_param = [0.0, 0.0, 0.0, 0.0, 0.0]
        self.shape_points = 0
        self.points = []

    def set(self, type, name, param=""):
        self.type = type
        self.name = name
        self.param = param

    def set_shape(self, shape, rgba, rgba1, shape_param, shape_points, points):
        self.type = 3
        self.rgba = rgba
        self.rgba2 = rgba1
        self.shape = shape
        self.shape_param = shape_param
        self.shape_points = shape_points
        self.points = points

    def set_path(self, shape, rgba, arr):
        self.type = 3
        self.rgba = rgba
        self.shape = shape
        thick, shape_points, points = arr
        self.shape_param = [thick, 0.0, 0.0, 0.0, 0.0]
        self.shape_points = shape_points
        self.points = points

    def clone(self, msg):
        self.type = msg.type
        self.name = msg.name
        self.param = msg.param
        self.type = msg.type
        self.rgba = msg.rgba
        self.rgba2 = msg.rgba2
        self.shape = msg.shape
        self.shape_param = msg.shape_param
        self.shape_points = msg.shape_points
        self.points = msg.points

    def add_point(self, x, y):
        self.shape_points = self.shape_points + 1
        self.points.append(x)
        self.points.append(y)

    def get_type(self):
        return self.type

    def get_name(self):
        return self.name

    def get_param(self):
        return self.param

    def get_shape_type(self):
        return self.shape

    def get_shape(self):
        return [self.rgba, self.rgba2, self.shape_param, self.shape_points]

    def get_path(self):
        return [self.rgba, self.shape_param[0], self.shape_points, self.points]
