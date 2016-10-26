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
        Connecting=0xF89EC8,// 16293576
        LoggedIn=0xF1A314,// 15835924
        InWorld=0xEAB6D9,// 15382233
        CharSelecting=0xD82F24,// 14167844
        CharacterNumber=0xD82F38,// 14167864
        CharacterOffset=0xD82F3C// 14167868
    };
    enum FrameManager2
    {
        FrameBase=0xD0ADA8,// 13675944
        ScreenWidth=0xBF23D4,// 12526548
        ScreenHeight=0xBF23D8,// 12526552
        GetRegionType_FontString=0xDFED3,// 917203
        CurrentKeyBoardFocusFrame=0xCFEA10,// 13625872
        FrameName=0x14,// Frame__GetName
        FirstFrame=0x6C0,// (P)Script_EnumerateFrames+12
        NextFrame=0x138,// (P)Script_EnumerateFrames+29
        FrameBottom=0x4C,// (P)Script__Frame_GetBottom+13 :+ (P)GetFrameSize+10
        FrameLeft=0x50,// (P)Script__Frame_GetLeft+13 :+ (P)GetFrameSize+11
        FrameTop=0x54,// (P)Script__Frame_GetTop+13 :+ (P)GetFrameSize+13
        FrameRight=0x58,// (P)Script__Frame_GetRight+13 :+ (P)GetFrameSize+14
        FrameId=0x94,// Script__Frame_GetID+016
        FrameParent=0x84,// Frame__GetParent
        FirstRegion=0x118,// (P)GetFrameRegions+17
        NextRegion=0x110,// (P)GetFrameRegions+23
        FontStringRegionText=0xE4,// (P)GetRegionText+7
        GetRegionTypeVTableOffset=0x18// (P)GetRegionType+9
    };
    enum ObjectManager2
    {
        ObjectManager=0xD98168,// 14254440
        ActivePlayer=0xE34860,// 14895200
        FirstObject=0xD8,// (P)ClntObjMgrEnumVisibleObjectsPtr+11
        NextObject=0xD0// (P)ClntObjMgrEnumVisibleObjectsPtr+17
    };
    enum Unit2
    {
        UnitCache=0xC80,// (P)CGUnit_C__GetUnitName+203
        UnitCacheName=0x80,// (P)CGUnit_C__GetUnitNameAux+9
        Position=0x124,// (P)CGUnit_C__GetPosition+2
        Coordinates=0x18,// (P)CGUnit_C__GetPosition+2
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
