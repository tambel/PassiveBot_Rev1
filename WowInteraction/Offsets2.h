#pragma once
namespace WowOffsets2
{
    enum Client
    {
        Connecting=0xF3A200,// 15966720
        LoggedIn=0xECA9C0,// 15509952
        CharSelecting=0xD39398// 13865880
    };
    enum FrameManager2
    {
        FrameBase=0xCC2218,// 13378072
        ScreenWidth=0xBAA48C,// 12231820
        ScreenHeight=0xBAA490,// 12231824
        FrameName=0x14,// Frame__GetName
        FirstFrame=0x684,// Script__EnumerateFrames+0x69
        NextFrame=0x150,// Script__EnumerateFrames+0x88
        FrameBottom=0x4c,// Script__Frame_GetBottom+0x3B + GetFrameSize
        FrameLeft=0x50,// Script__Frame_GetLeft+0x3B + GetFrameSize
        FrameTop=0x54,// Script__Frame_GetTop+0x3B + GetFrameSize
        FrameRight=0x58,// Script__Frame_GetRight+0x3B + GetFrameSize
        FrameId=0x94,// Script__Frame_GetID+016
        FrameParent=0x84// Frame__GetParent
    };

}
