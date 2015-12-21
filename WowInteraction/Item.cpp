#include "stdafx.h"
using namespace Tools;
namespace Wow
{
	Item::Item(unsigned base):WowObject(base)
	{
	}


	Item::~Item(void)
	{
	}
	wchar_t * Item::GetName(bool refresh)
	{
		if (!name || refresh)
		{
			delete [] name;
			name = Process::ReadString_UTF8(Process::ReadUInt(Process::ReadRelUInt(WowOffsets::Item::CachePtr+0x28)+4*(GetID()-Process::ReadRelUInt(WowOffsets::Item::CachePtr+0x14)))+WowOffsets::Item::ItemNameoffset,0);
		}
		return name;
	}
}
