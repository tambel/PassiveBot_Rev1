from collections import OrderedDict
names=OrderedDict()
names["Client"]=['Connecting','LoggedIn','InWorld','CharSelecting','CharacterNumber','CharacterOffset']
names["FrameManager2"]=['FrameBase','ScreenWidth','ScreenHeight','GetRegionType_FontString', 'CurrentKeyBoardFocusFrame',
                       ('FrameName',0x14, 'Frame__GetName'),
                       ('FirstFrame', 0x684,'Script_EnumerateFrames+0x69'),
                       ('NextFrame',0x150,'Script_EnumerateFrames+0x88'),
                       ('FrameBottom', 0x4C,'(P)Script__Frame_GetBottom+13 :+ (P)GetFrameSize+10'),
                       ('FrameLeft',0x50,'(P)Script__Frame_GetLeft+13 :+ (P)GetFrameSize+11'),
                       ('FrameTop',0x54,'(P)Script__Frame_GetTop+13 :+ (P)GetFrameSize+13'),
                       ('FrameRight',0x58,'(P)Script__Frame_GetRight+13 :+ (P)GetFrameSize+14'),
                       ('FrameId',0x94,'Script__Frame_GetID+016'),
                       ('FrameParent', 0x84, 'Frame__GetParent'),
                       ('FirstRegion', 0x130, '(P)GetFrameRegions+19'),
                       ('NextRegion', 0x128, '(P)GetFrameRegions+25'),
                       ('FontStringRegionText', 0xE4, '(P)GetRegionText+7'),
                       ('GetRegionTypeVTableOffset', 0x20, '(P)GetRegionType+9'),
                       ]
names["ObjectManager2"]=['ObjectManager','ActivePlayer',
                        ('FirstObject',0xD8, '(P)ClntObjMgrEnumVisibleObjectsPtr+11'),
                        ('NextObject', 0xD0, '(P)ClntObjMgrEnumVisibleObjectsPtr+17'),
                        ]
names["Unit2"]=[
                ('UnitCache', 0xC80,'(P)CGUnit_C__GetUnitName+203'),
                ('UnitCacheName', 0x80, '(P)CGUnit_C__GetUnitNameAux+9'),
                ('Position',0x124, '(P)CGUnit_C__GetPosition+2'),
                ('Coordinates',0x18, '(P)CGUnit_C__GetPosition+2'),
                ('Rotation', 0x28),
                ]
names["Player2"]=[ ('PlayerNameCache', 0xD10834),
                 ('PlayerNameCacheNext', 0x00),
                 ('PlayerNameCacheGuid', 0x10),
                 ('PlayerNameCacheName', 0x21),
                 ('PlayerNameCacheRace', 0x70),
                 ('PlayerNameCacheClass', 0x78),
                 ]
names["GameObject2"]=[('ObjectCache', 0x26C),
                    ('ObjectName', 0xB4),
                    ]

		






import idautils, idc,idaapi
offsets=OrderedDict()
for k,v in names.items():
    offsets[k]=OrderedDict()
    for n in v:
        if type(n)==tuple:
            if len(n)==3:
                offsets[k][n[0]] =(hex(n[1]).upper().replace('X','x'),n[2])
            else:
                offsets[k][n[0]] =hex(n[1]).upper().replace('X','x')
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

legend_string = "/*" \
                "Determined offsets comments:\n" \
                "   -If <FUNCTION_NAME> looks like <FUNCTION_NAME + LINE_OFFSET> then offset is in function address + LINE_OFFSET.\n" \
                "    LINE_OFFSET is decimal when its in pseudocode.\n" \
                "   -(P)<FUNCTION_NAME> - watch pseudocode of the function.\n" \
                "   -<FUNCTION_NAME1>:+<FUNCTION_NAME2> -  final offset is a sum of other offsets in <FUNCTION_NAME1> and <FUNCTION_NAME2>.\n" \
                "*/\n\n"

result_string = legend_string+"#pragma once\nnamespace WowOffsets2\n{\n"+enums+"\n}\n"

output.write(result_string)
output.close()