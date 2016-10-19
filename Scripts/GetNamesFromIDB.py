from collections import OrderedDict
names=OrderedDict()
names["Client"]=['Connecting','LoggedIn','CharSelecting']
names["FrameManager"]=['FrameBase',
                       ('FrameName',0x14, 'БЛАБЛАБЛА'),
                       ('FirstFrame', 0x684),
                       ('NextFrame',0x150)
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
