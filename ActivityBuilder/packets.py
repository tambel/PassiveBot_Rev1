import struct
import collections
from struct import *
from game_types import *

class Packet(object):

    def __init__(self,type=0):
        self.size=0
        self.type=type
        self.fields=collections.OrderedDict()

    def pack(self, *args):
        self.size=8+sum([f.type.size for f in self.fields.values()])
        return pack("II",self.size,self.type)+"".join([pack(f.type.sig, f.value) for f in self.fields.values()])

    def unpack(self,data):
        size,data=data
        self.size=size
        self.type,payload= unpack('I'+"{}s".format(size-8),data)
        pformat=''.join([f.TYPE[0] for f in self.fields.values()])
        for k,v in zip(self.fields.keys(),unpack(pformat,payload)):
            self.fields[k]=self.fields[k](v)
        '''
        other_data_size=len(data)-8
        self.size,self.type,payload= unpack('II'+"{}s".format(other_data_size),data)
        pformat=''.join([f.TYPE[0] for f in self.fields.values()])
        for k,v in zip(self.fields.keys(),unpack(pformat,payload)):
            self.fields[k]=self.fields[k](v)
        '''

class RequestPacket(Packet):
    def __init__(self,type):
        Packet.__init__(self,type=type)



class PlayerPositionReply(Packet):
    def __init__(self,data):
        Packet.__init__(self)
        self.fields['position']=Vector3
        self.fields['rotation']=Vector3
        self.unpack(data)





class TargerObjevtInfoReply(Packet):
    def __init__(self,data):
        Packet.__init__(self)
        self.fields['guid']=GUID
        self.fields['type']=Char
        self.fields['position']=Position
        self.fields['name']=SmallString
        self.unpack(data)

