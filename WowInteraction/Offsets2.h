/*Determined offsets comments:
   -If <FUNCTION_NAME> looks like <FUNCTION_NAME + LINE_OFFSET> then offset is in function address + LINE_OFFSET.
    LINE_OFFSET is decimal when its in pseudocode.
   -(P)<FUNCTION_NAME> - watch pseudocode of the function.
   -<FUNCTION_NAME1>:+<FUNCTION_NAME2> -  final offset is a sum of other offsets in <FUNCTION_NAME1> and <FUNCTION_NAME2>.
*/

#pragma once
namespace WowOffsets2
{
    enum Client
    {
        Connecting=0xF3A200,// 15966720
        LoggedIn=0xECA9C0,// 15509952
        InWorld=0xE5BD19,// 15056153
        CharSelecting=0xD39398,// 13865880
        CharacterNumber=0xD393AC,// 13865900
        CharacterOffset=0xD393B0// 13865904
    };
    enum FrameManager2
    {
        FrameBase=0xCC2218,// 13378072
        ScreenWidth=0xBAA48C,// 12231820
        ScreenHeight=0xBAA490,// 12231824
        GetRegionType_FontString=0xCDAA3,// 842403
        CurrentKeyBoardFocusFrame=0xCB5F14,// 13328148
        FrameName=0x14,// Frame__GetName
        FirstFrame=0x684,// Script_EnumerateFrames+0x69
        NextFrame=0x150,// Script_EnumerateFrames+0x88
        FrameBottom=0x4C,// (P)Script__Frame_GetBottom+13 :+ (P)GetFrameSize+10
        FrameLeft=0x50,// (P)Script__Frame_GetLeft+13 :+ (P)GetFrameSize+11
        FrameTop=0x54,// (P)Script__Frame_GetTop+13 :+ (P)GetFrameSize+13
        FrameRight=0x58,// (P)Script__Frame_GetRight+13 :+ (P)GetFrameSize+14
        FrameId=0x94,// Script__Frame_GetID+016
        FrameParent=0x84,// Frame__GetParent
        FirstRegion=0x130,// (P)GetFrameRegions+19
        NextRegion=0x128,// (P)GetFrameRegions+25
        FontStringRegionText=0xE4,// (P)GetRegionText+7
        GetRegionTypeVTableOffset=0x20// (P)GetRegionType+9
    };
    enum ObjectManager2
    {
        ObjectManager=0xD4E578,// 13952376
        ActivePlayer=0xDE565C,// 14571100
        FirstObject=0xD8,// (P)ClntObjMgrEnumVisibleObjectsPtr+11
        NextObject=0xD0// (P)ClntObjMgrEnumVisibleObjectsPtr+17
    };
    enum Unit2
    {
        UnitCache=0xC80,// (P)CGUnit_C__GetUnitName+203
        UnitCacheName=0x80,// (P)CGUnit_C__GetUnitNameAux+9
        Position=0x124,// CGUnit_C__GetPosition+2
        Coordinates=0x18,// CGUnit_C__GetPosition+2
        Rotation=0x28// 40
    };
    enum Player2
    {
        PlayerNameCache=0xD10834,// 13699124
        PlayerNameCacheNext=0x0,// 0
        PlayerNameCacheGuid=0x10,// 16
        PlayerNameCacheName=0x21,// 33
        PlayerNameCacheRace=0x70,// 112
        PlayerNameCacheClass=0x78// 120
    };
    enum GameObject2
    {
        ObjectCache=0x26C,// 620
        ObjectName=0xB4// 180
    };

}
