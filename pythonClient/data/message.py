class Message():

    def __init__(self, type=0, name="", param=""):
        self.type = type
        self.name = name
        self.param = param

    def set(self, type, name, param=""):
        self.type = type
        self.name = name
        self.param = param

    def get_type(self):
        return self.type

    def get_name(self):
        return self.name

    def get_param(self):
        return self.param
