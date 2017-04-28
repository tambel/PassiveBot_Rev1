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
        Connecting=0x101F058,// 16904280
        LoggedIn=0xFACD40,// 16436544
        InWorld=0xF3DEEE,// 15982318
        CharSelecting=0xDF254C,// 14624076
        CharacterNumber=0xDF2A6C,// 14625388
        CharacterOffset=0xDF2A70// 14625392
    };
    enum FrameManager2
    {
        FrameBase=0xD82BD0,// 14166992
        ScreenWidth=0xC6B3FC,// 13022204
        ScreenHeight=0xC6B400,// 13022208
        UIObject__GetObjectType__FontString=0xEFED1,// 982737
        CurrentKeyBoardFocusFrame=0xD76808,// 14116872
        FrameName=0x14,// Frame__GetName
        FirstFrame=0x6B8,// (P)Script_EnumerateFrames+11
        NextFrame=0x13C,// (P)Script_EnumerateFrames+29
        FrameBottom=0x4C,// (P)Script__Frame_GetBottom+13 :+ (P)GetFrameSize+10
        FrameLeft=0x50,// (P)Script__Frame_GetLeft+13 :+ (P)GetFrameSize+11
        FrameTop=0x54,// (P)Script__Frame_GetTop+13 :+ (P)GetFrameSize+13
        FrameRight=0x58,// (P)Script__Frame_GetRight+13 :+ (P)GetFrameSize+14
        FrameId=0x94,// Script__Frame_GetID+016
        FrameParent=0x84,// Frame__GetParent
        FirstRegion=0x11C,// (P)GetFrameRegions+17
        NextRegion=0x114,// (P)GetFrameRegions+23
        FontStringRegionText=0xE8,// (P)GetRegionText+7
        GetRegionTypeVTableOffset=0x1C,// (P)GetRegionType+9
        FrameIsShown=0x80// 
    };
    enum ObjectManager2
    {
        ObjectManager=0xE0A97C,// 14723452
        ActivePlayer=0xEC4E94,// 15486612
        FirstObject=0xD8,// (P)ClntObjMgrEnumVisibleObjectsPtr+11
        NextObject=0xD0,// (P)ClntObjMgrEnumVisibleObjectsPtr+17
        MouseGuid=0xEAC500,// Search by hand
        TargetGuid=0xF9C3E8,// Search by hand
        IsResting=0x0// Script_IsResting
    };
    enum Unit2
    {
        UnitCache=0xC68,// (P)CGUnit_C__GetUnitName+203
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
    enum Object2
    {
        EntityID=0x24// 36
    };

}
