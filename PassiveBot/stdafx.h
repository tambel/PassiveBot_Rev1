// stdafx.h: включаемый файл дл€ стандартных системных включаемых файлов
// или включаемых файлов дл€ конкретного проекта, которые часто используютс€, но
// не часто измен€ютс€
//

#pragma once

#include "WowInteraction\stdafx.h"
#include "WowWorldsData\stdafx.h"
#include "BotInteractor\stdafx.h"



struct dbc_header
{
	unsigned magic; // always 'WDBC'
	unsigned record_count; // records per file
	unsigned field_count; // fields per record
	unsigned record_size; // sum (sizeof (field_type_i)) | 0 <= i < field_count. field_type_i is NOT defined in the files.
	unsigned string_block_size;
};

struct db2_header
{
	unsigned magic;                                               // 'WDB2' for .db2 (database), 'WCH3' for .adb (cache)
	unsigned record_count;
	unsigned field_count;
	unsigned record_size;
	unsigned string_table_size;                                   // string block almost always contains at least one zero-byte
	unsigned table_hash;
	unsigned build;
	unsigned timestamp_last_written;                              // set to time(0); when writing in WowClientDB2_Base::Save()
	unsigned min_id;
	unsigned max_id;
	unsigned locale;                                              // as seen in TextWowEnum
	unsigned copy_table_size;
};

struct QuestPOIBlobRec {
	unsigned m_ID;
	unsigned m_NumPoints;
	unsigned m_MapID;
	unsigned m_WorldMapAreaID;
};

struct QuestPOIPointRec {
	unsigned m_ID;
	unsigned m_X;
	unsigned m_Y;
	unsigned m_QuestPOIBlobID;
};
struct WorldMapAreaRec {
	unsigned m_ID;
	unsigned m_mapID;
	unsigned m_areaID;
	unsigned m_areaName;
	float m_locLeft;
	float m_locRight;
	float m_locTop;
	float m_locBottom;
	unsigned m_displayMapID;
	unsigned m_defaultDungeonFloor;
	unsigned m_parentWorldMapID;
	unsigned m_flags;                     // & 8: skip .zmp file: assume all zero.
	unsigned m_levelRangeMin;
	unsigned m_levelRangeMax;
};

//struct AreaTableRec {
//	char f[29][4];
//};

struct AreaTableRec {
	unsigned m_ID;
	unsigned m_ContinentID;
	unsigned m_ParentAreaID;
	unsigned m_AreaBit;
	unsigned m_flags[2]; // 0x100000000: isMicroDungeon
	unsigned m_SoundProviderPref;
	unsigned m_SoundProviderPrefUnderwater;
	unsigned m_AmbienceID;
	unsigned m_ZoneMusic;
	unsigned m_ZoneName;
	unsigned m_IntroSound;
	unsigned m_ExplorationLevel;
	unsigned m_AreaName_lang;
	unsigned m_factionGroupMask;
	unsigned m_liquidTypeID[4];
	float m_minElevation;
	float m_ambient_multiplier;
	unsigned m_lightid;
	unsigned m_mountFlags;
	unsigned m_uwIntroSound;
	unsigned m_uwZoneMusic;
	unsigned m_uwAmbience;
	unsigned m_world_pvp_id;
	unsigned m_pvpCombatWorldStateID;
	unsigned m_wildBattlePetLevelMin;
	//unsigned m_wildBattlePetLevelMax;
	//unsigned m_windSettingsID;
};
// TODO: ”становите здесь ссылки на дополнительные заголовки, требующиес€ дл€ программы
