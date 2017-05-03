#pragma once
struct wdb5_db2_header
{
	unsigned magic;                                               // 'WDB5' for .db2 (database)
	unsigned record_count;
	unsigned field_count;                                         // for the first time, this counts arrays as '1'; in the past, only the WCH* variants have counted arrays as 1 field
	unsigned record_size;
	unsigned string_table_size;                                   // if flags & 0x01 != 0, this field takes on a new meaning - it becomes an absolute offset to the beginning of the offset_map
	unsigned table_hash;
	unsigned layout_hash;                                         // used to be 'build', but after build 21737, this is a new hash field that changes only when the structure of the data changes
	unsigned min_id;
	unsigned max_id;
	unsigned locale;                                              // as seen in TextWowEnum
	unsigned copy_table_size;
	unsigned short flags;                                               // in WDB3/WCH4, this field was in the WoW executable's DBCMeta; possible values are listed in Known Flag Meanings
	unsigned short id_index;                                            // new in WDB5 (and only after build 21737), this is the index of the field containing ID values; this is ignored if flags & 0x04 != 0
};

