import struct
import collections
from struct import *
from game_types import *

'''
class Requests(object):
    PlayerPosition = 1
    TargetInfo = 2
    Logout = 3
    TargetQuestGiverQuestList = 4
    SelectQuestFromList = 5
    TargetEntityID = 6
'''




class Packet(object):
    field_types=collections.OrderedDict()

    def __init__(self):
        self.size=0
        self.type=0
        self.fields={}
        #self.type, payload = unpack('I' + "{}s".format(size - 8), data)


    def pack(self):
        self.size = 8 + sum([ft.size for ft in self.field_types.values()])
        return pack("II", self.size,self.type)

    def unpack(self, size, data):
        self.type, payload=struct.unpack("I"+ "{}s".format(size - 8), data)
        payload_format=''.join([f.TYPE[0] for f in self.field_types.values()])
        for k, v in zip(self.field_types.keys(), unpack(payload_format, payload)):
            self.fields[k]=self.field_types[k](v)
        return self






'''
class Packet(object):
    def __init__(self, structure, packet_type=0, data=None):
        self.size = 0
        self.type = packet_type
        self.structure = structure
        if data is not None:
            self.unpack(data, structure)
            # self.fields = collections.OrderedDict()

    def pack(self):
        self.size = 8 + sum([f.type.size for f in self.structure.values()])
        return pack("II", self.size,
                    self.type)  # + "".join([pack(v.TYPE[0], getattr(v, k)) for k, v in structure.items()])

    def unpack(self, data, structure):
        size, data = data
        self.size = size
        self.type, payload = unpack('I' + "{}s".format(size - 8), data)
        pformat = ''.join([f.TYPE[0] for f in self.structure().values()])
        for k, v in zip(Structures[self.structure].keys(), unpack(pformat, payload)):
            # self.fields[k] = self.fields[k](v)
            setattr(self, k, Structures[self.structure][k](v))
'''



class PacketStructure(collections.OrderedDict):
    def __init__(self):
        collections.OrderedDict.__init__(self)


class RequestPacket(Packet):
    def __init__(self, request_type):
        Packet.__init__(self)
        self.type=request_type if type(request_type) is int else Requests[request_type]


class PlayerPosition(PacketStructure):
    def __init__(self):
        PacketStructure.__init__(self)
        self['position'] = Vector3
        self['rotation'] = Vector3


class TargetObjectInfo(Packet):
    field_types=collections.OrderedDict([('guid', GUID), ('entity_id', Unsigned),('type', Char), ('position', Position),('name', SmallString)])


class TargetQuestGiverQuestList(Packet):
    field_types = collections.OrderedDict()
    field_types["count"]=Unsigned
    field_types["quest_detail_triggered"]=Char
    field_types["id"]=Unsigned
    for i in range(0, 20):
        field_types['name{}'.format(i)] = SmallString


class SelectFromQuestList(Packet):
    field_types = collections.OrderedDict()
    field_types['id'] = Unsigned
    field_types['title'] = SmallString



class TargetEntityId(PacketStructure):
    def __init__(self):
        PacketStructure.__init__(self)
        self['id'] = Unsigned


Requests = {
    PlayerPosition: 1,
    TargetObjectInfo: 2,
    "Logout": 3,
    TargetQuestGiverQuestList: 4,
    "SelectQuestFromList": 5,
    "TargetEntityID": 6
}
