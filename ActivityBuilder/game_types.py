from struct import *
import collections


class Field(object):
    def __init__(self,type,size=None):
        self.value=None
        self.type=type
        self.size=size

class Simple(object):
    TYPE =None
    def __init__(self, data):
        self.value=data

    def __str__(self):
        return str(self.value)

class Char(Simple):
     TYPE=("B",)

class Unsigned(Simple):
    TYPE=("I",)

def gen_filler(size):
    class f(Filler):
        pass
    f.TYPE=("{}s".format(size),size)
    return f

class Filler(object):
    def __init__(self,data):
        pass


class Vector3(object):
    TYPE = ("12s",)
    def __init__(self, data=None, x=0, y=0, z=0):
        if data is not None:
            x, y, z = unpack("fff", data)
        self.x = x
        self.y = y
        self.z = z

    def __str__(self):
        return "x: {}, y: {}, z: {}".format(self.x, self.y, self.z)


class Position(object):
    TYPE = ("24s",)

    def __init__(self, data=None, p=None, r=None):
        if data is not None:
            pd, rd = unpack("12s12s", data)
            p = Vector3(pd)
            r = Vector3(rd)
        self.position = p
        self.rotation = r

    def __str__(self):
        return "Coordinates: {}\nRotation: {}".format(self.position, self.rotation)


class GUID(object):
    TYPE = ("16s",)

    def __init__(self, data):
        self.high, self.low = unpack("QQ", data)

    def __str__(self):
        return "high: {}, low: {}".format(self.high,self.low)


class SmallString(object):
    TYPE=("260s",)
    def __init__(self,data):
        self.length,data=unpack("I256s",data)
        data=data[:self.length]
        self.text=data.decode('utf-16')
        a=10
    def __unicode__(self):
        return self.text



