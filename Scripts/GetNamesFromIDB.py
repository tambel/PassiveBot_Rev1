from collections import OrderedDict
names=OrderedDict()
names["Client"]=['Connecting','LoggedIn','CharSelecting','CharacterNumber','CharacterOffset']
names["FrameManager"]=['FrameBase','ScreenWidth','ScreenHeight',
                       ('FrameName',0x14, 'Frame__GetName'),
                       ('FirstFrame', 0x684,'Script__EnumerateFrames+0x69'),
                       ('NextFrame',0x150,'Script__EnumerateFrames+0x88'),
                       ('FrameBottom', 0x4C,'Script__Frame_GetBottom+0x3B + GetFrameSize'),
                       ('FrameLeft',0x50,'Script__Frame_GetLeft+0x3B + GetFrameSize'),
                       ('FrameTop',0x54,'Script__Frame_GetTop+0x3B + GetFrameSize'),
                       ('FrameRight',0x58,'Script__Frame_GetRight+0x3B + GetFrameSize'),
                       ('FrameId',0x94,'Script__Frame_GetID+016'),
                       ('FrameParent', 0x84, 'Frame__GetParent')
                       ]
                  






import idautils, idc,idaapi
offsets=OrderedDict()
for k,v in names.items():
    offsets[k]=OrderedDict()
    for n in v:
        if type(n)==tuple:
            if len(n)==3:
                offsets[k][n[0]] =(hex(n[1]),n[2])
            else:
                offsets[k][n[0]] =hex(n[1])
        else:
            addr_string=hex(idc.LocByName(n)-idaapi.get_imagebase())
            addr_string=addr_string.replace('L','').upper().replace('X','x')
            offsets[k][n] =addr_string

output=open("C:\Users\laptop\Documents\Visual Studio 2015\Projects\PassiveBot_Rev1\WowInteraction\Offsets2.h",'w')


enums=""
for node_key,node in offsets.items():
    enums+="    enum {}\n    {{\n".format(node_key)
    couter=0
    for name,v in  node.items():
        if type(v)==tuple:
            addr=v[0]
            comment="// {}\n".format(v[1])
        else:
            addr=v
            comment="// {}\n".format(int(addr,16))
        enums+="        {name}={addr}".format(name=name,addr=addr)
        if couter<len(node)-1:
            enums+=","
        enums+=comment
        couter+=1
    enums+="    };\n"

result_string="#pragma once\nnamespace WowOffsets2\n{\n"+enums+"\n}\n"

output.write(result_string)
output.close()
