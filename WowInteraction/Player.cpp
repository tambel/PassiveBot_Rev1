#include "stdafx.h"
using namespace Tools;

Player::Player(unsigned base) :Unit(base)
{
}


Player::~Player(void)
{
}
wstring & Player::GetName(bool refresh)
{
	if (refresh)
	{
		Guid128 tmp_guid = Guid128();
		unsigned current = Process::ReadRel<unsigned>(WowOffsets::Player::PlayerNameCache);
		Process::ReadRaw(current + WowOffsets::PlayerNameCacheGuid, &tmp_guid, 16);
		while (*GetGuid() != tmp_guid)
		{
			current = Process::Read<unsigned>(current + WowOffsets::Player::PlayerNameCacheNext);
			Process::ReadRaw(current + WowOffsets::PlayerNameCacheGuid, &tmp_guid, 16);
		}
		name = Process::ReadString_UTF8(current + WowOffsets::PlayerNameCacheName, 0);

	}
	return name;
}
