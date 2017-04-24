from struct import *
import collections

OBJECT_TYPES = {'ITEM': 1,
                'CONTAINER': 2,
                'UNIT': 3,
                'PLAYER': 4,
                'GAMEOBJECT': 5,
                'DYNAMICOBJECT': 6,
                'CORPSE': 7,
                'AiGroup': 8,
                'AreaTrigger': 9}


class Field(object):
    def __init__(self, type, size=None):
        self.value = None
        self.type = type
        self.size = size


class Simple(object):
    TYPE = None

    def __init__(self, data):
        self.value = data

    def __str__(self):
        return str(self.value)


class Char(Simple):
    TYPE = ("B",)
    size=1


class Unsigned(Simple):
    TYPE = ("I",)
    size=4


def gen_filler(size):
    class f(Filler):
        pass

    f.TYPE = ("{}s".format(size), size)
    return f


class Filler(object):
    def __init__(self, data):
        pass


class Vector3(object):
    TYPE = ("12s",)
    size=12

    def __init__(self, data=None, x=0, y=0, z=0):
        if data is not None:
            x, y, z = unpack("fff", data)
        self.x = x
        self.y = y
        self.z = z

    def __str__(self):
        return "x:{}\ny: {}\nz: {}\n".format(self.x, self.y, self.z)

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y and self.z == other.z

    def __ne__(self, other):
        return not self.__eq__(other)


class Position(object):
    TYPE = ("24s",)
    size=24

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
    size=16

    def __init__(self, data):
        self.high, self.low = unpack("QQ", data)

    def __str__(self):
        return "high: {}, low: {}".format(self.high, self.low)


class SmallString(object):
    TYPE = ("260s",)
    size=260

    def __init__(self, data):
        self.length, data = unpack("I256s", data)
        data = data[:self.length]
        self.text = data.decode('utf-16')

    def __unicode__(self):
        return self.text
