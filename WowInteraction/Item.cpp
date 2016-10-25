#include "stdafx.h"
#include "Item.h"
using namespace Tools;

Item::Item(unsigned base) :WowObject(base)
{
}


Item::~Item(void)
{
}
wstring & Item::GetName(bool refresh)
{
	if (refresh)
	{
		name = Process::ReadString_UTF8(Process::Read<unsigned>(Process::ReadRel<unsigned>(WowOffsets::Item::CachePtr + 0x28) + 4 * (GetID() - Process::ReadRel<unsigned>(WowOffsets::Item::CachePtr + 0x14))) + WowOffsets::Item::ItemNameoffset, 0);
	}
	return name;
}

