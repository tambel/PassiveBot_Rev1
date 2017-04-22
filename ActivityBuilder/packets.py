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
        other_data_size=len(data)-8
        self.size,self.type,payload= unpack('II'+"{}s".format(other_data_size),data)
        pformat=''.join([f.TYPE[0] for f in self.fields.values()])
        for k,v in zip(self.fields.keys(),unpack(pformat,payload)):
            self.fields[k]=self.fields[k](v)
        '''


class PacketStructure(collections.OrderedDict):
    def __init__(self):
        collections.OrderedDict.__init__(self)


class RequestPacket(Packet):
    def __init__(self, request_type):
        Packet.__init__(self, structure={}, packet_type=request_type)


class PlayerPosition(PacketStructure):
    def __init__(self):
        PacketStructure.__init__(self)
        self['position'] = Vector3
        self['rotation'] = Vector3


class TargetObjectInfo(PacketStructure):
    def __init__(self):
        PacketStructure.__init__(self)
        self['guid'] = GUID
        self['entity_id'] = Unsigned
        self['type'] = Char
        self['position'] = Position
        self['name'] = SmallString


class TargetQuestGiverQuestList(PacketStructure):
    def __init__(self):
        PacketStructure.__init__(self)
        self['count'] = Unsigned
        for i in range(0, 20):
            self['name{}'.format(i)] = SmallString


class SelectFromQuestList(PacketStructure):
    def __init__(self):
        PacketStructure.__init__(self)
        self['id'] = Unsigned
        self['title'] = SmallString


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

Structures = {k: k() if type(k) != str else None for k in Requests.keys()}
for k in Structures.keys():
    if Structures[k] is None:
        Structures.pop(k)

