class Message():

    def __init__(self, type=0, name=0, param=""):
        self.typ e = type
        self.nam e = name
        self.para m = param

    def set(self, type, name, param=""):
        self.typ e = type
        self.nam e = name
        self.para m = param

    def get_type(self):
        return self.type

    def get_name(self):
        return self.name

    def get_param(self):
        return self.param
