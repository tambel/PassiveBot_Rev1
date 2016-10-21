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
        CharSelecting=0xD39398,// 13865880
        CharacterNumber=0xD393AC,// 13865900
        CharacterOffset=0xD393B0// 13865904
    };
    enum FrameManager2
    {
        FrameBase=0xCC2218,// 13378072
        ScreenWidth=0xBAA48C,// 12231820
        ScreenHeight=0xBAA490,// 12231824
        FrameName=0x14,// Frame__GetName
        FirstFrame=0x684,// Script__EnumerateFrames+0x69
        NextFrame=0x150,// Script__EnumerateFrames+0x88
        FrameBottom=0x4c,// (D)Script__Frame_GetBottom+13 :+ (D)GetFrameSize+10
        FrameLeft=0x50,// (D)Script__Frame_GetLeft+13 :+ (D)GetFrameSize+11
        FrameTop=0x54,// (D)Script__Frame_GetTop+13 :+ (D)GetFrameSize+13
        FrameRight=0x58,// (D)Script__Frame_GetRight+13 :+ (D)GetFrameSize+14
        FrameId=0x94,// Script__Frame_GetID+016
        FrameParent=0x84,// Frame__GetParent
        FirstRegion=0x130,// (D)GetFrameRegions+19
        NextRegion=0x128,// (D)GetFrameRegions+25
        FontStringRegionText=0xe4// (D)GetRegionText+7
    };

}
