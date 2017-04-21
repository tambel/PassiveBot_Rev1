from collections import OrderedDict
import os
names=OrderedDict()
names["Client"]=['Connecting','LoggedIn','InWorld','CharSelecting','CharacterNumber','CharacterOffset']
names["FrameManager2"]=['FrameBase','ScreenWidth','ScreenHeight','GetRegionType_FontString', 'CurrentKeyBoardFocusFrame',
                       ('FrameName',0x14, 'Frame__GetName'),
                       ('FirstFrame', 1728,'(P)Script_EnumerateFrames+12'),
                       ('NextFrame',312,'(P)Script_EnumerateFrames+29'),
                       ('FrameBottom', 0x4C,'(P)Script__Frame_GetBottom+13 :+ (P)GetFrameSize+10'),
                       ('FrameLeft',0x50,'(P)Script__Frame_GetLeft+13 :+ (P)GetFrameSize+11'),
                       ('FrameTop',0x54,'(P)Script__Frame_GetTop+13 :+ (P)GetFrameSize+13'),
                       ('FrameRight',0x58,'(P)Script__Frame_GetRight+13 :+ (P)GetFrameSize+14'),
                       ('FrameId',0x94,'Script__Frame_GetID+016'),
                       ('FrameParent', 0x84, 'Frame__GetParent'),
                       ('FirstRegion', 280, '(P)GetFrameRegions+17'),
                       ('NextRegion', 272, '(P)GetFrameRegions+23'),
                       ('FontStringRegionText', 228, '(P)GetRegionText+7'),
                       ('GetRegionTypeVTableOffset', 24, '(P)GetRegionType+9'),
                       ('FrameIsShown', 128,''),
                       ]
names["ObjectManager2"]=['ObjectManager','ActivePlayer',
                        ('FirstObject',0xD8, '(P)ClntObjMgrEnumVisibleObjectsPtr+11'),
                        ('NextObject', 0xD0, '(P)ClntObjMgrEnumVisibleObjectsPtr+17'),
                        ('MouseGuid', 0xEAC500, "Search by hand"),
                        ('TargetGuid', 0xF09C00, "Search by hand"),
                        ('IsResting' ,0x0, 'Script_IsResting'),
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

names["Object2"]=[('EntityID', 0x24)]

		






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

#path=os.path.join("D:\"Bot\Projects\PassiveBot\WowInteraction")
#print(path)
#output=open(path,'w')
output=open("D:\Bot\Projects\PassiveBot\WowInteraction\Offsets2.h",'w')


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
