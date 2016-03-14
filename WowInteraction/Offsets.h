#pragma once
namespace WowOffsets
{
	enum Client
	{
		Loaded = 0xD882DC, //CGlueMgr__CanLogIn
		Unavalible = 0xD871A0,//CGlueMgr__CanLogIn
		CharactersNumber = 0xD87988, //Script_GetNumCharacters
		CharactersOffset = 0xD8798C,
		InWorld = 0xEAEACA,


	};
	enum Camera
	{
		CameraPointer = 0xEAF270,
		CameraOffset = 0x7610,
		CameraPosition = 0x8,
		CameraMatrix = 0x14,
		CameraFOV = 0x38,
		FarClip = 0xF8421C, //World__GetFarClip
		NearClip = 0xCD3660 //World__GetNearClip
	};
	enum FrameManager
	{
		ScrWidth = 0xC5ADC8, // + 0xB000
		ScrHeight = 0xC5ADCC, // + 0xB000
		FrameBase = 0xD1A2DC, // + 0xC1C8 //variable -  CSimpleTop__s_instance
		CurrentFramePtr = 0xD1A25C, // + 0xC1C8

		FirstFrame = 0x12F4,
		NextFrame = 0x12EC,
		RegionsFirst = 0x168,
		RegionsNext = 0x160,
		Visible = 0x64,
		Visible1 = 0x1A,
		Visible2 = 1,
		LabelText = 0xF8,
		Name = 0x1C,

		ButtonEnabledPointer = 0x1F4,   //Assumed Good
		ButtonEnabledMask = 0xF,        //Assumed Good
		ButtonChecked = 0x230,          //Assumed Good
		EditBoxText = 0x210,            //Assumed Good
		FrameBottom = 0x68,             //Assumed Good
		FrameLeft = 0x6c,               //Assumed Good
		FrameTop = 0x70,                //Assumed Good
		FrameRight = 0x74,              //Assumed Good
		CurrentFrameOffset = 0x88,      //Assumed Good?  
		ParentFrame = 0x98

	};
	enum ObjectManager
	{
		ObjectPanagerPtr = 0xFEFA58,
		ObjectManagerOffset = 0x62C,
		FirstObject = 0xD8,
		NextObject = 0x3C,
		LocalPlayer = 0xE379B0


	};
	enum Object
	{
		Type = 0xC,
		Descriptors = 0x4,
		GUID = 0x0,
		EntityID = 0x24

	};
	enum GameObject
	{
		GameObjectNameCache = 0x274,
		GameObjectNameOffset = 0xB4
	};
	enum Item
	{
		CachePtr = 0xD70250,//WowClientDB2__ItemRecSparse_C__GetRecord+25
		ItemNameoffset = 0x198
	};
	enum Unit
	{
		UnitNameCache = 0xC38,
		UnitNameOffset = 0x7C,
		UnitPosition = 0xAC0,
		UnitRotation = 0xAD0
	};
	enum Player
	{
		PlayerNameCache = 0xD7BF14,
		PlayerNameCacheNext = 0x00,
		PlayerNameCacheGuid = 0x10,
		PlayerNameCacheName = 0x21,
		PlayerNameCacheRace = 0x70,
		PlayerNameCacheClass = 0x78
	};
	enum Quest
	{
		//QuestNumber = 0xF52F10
		QuestNumber = 0xF52F28,
		ExtendedQuestNumber = 0xF52F10,
		UnknonByteList = 0xF528DC,
		QuestIdList = 0xf528d0,
		QuestCache = 0xD7BC28,
		QuestBlobInfo1 = 0xF5CEB8,
		JournalSelectedQuest = 0xF52F1C
		//QuestInfos1= 0xF52F78

	};
}