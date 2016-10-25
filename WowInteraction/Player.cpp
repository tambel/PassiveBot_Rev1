#include "stdafx.h"
#include "Player.h"
using namespace Tools;

Player::Player(unsigned base) :Unit(base)
{
}


Player::~Player(void)
{
}
const wstring & Player::GetName(bool refresh)
{
	if (name.length()==0 || refresh)
	{
		Guid128 tmp_guid = Guid128();
		unsigned current = Process::ReadRel<unsigned>(WowOffsets2::Player2::PlayerNameCache);
		Process::ReadRaw(current + WowOffsets2::Player2::PlayerNameCacheGuid, &tmp_guid, 16);
		while (*GetGuid() != tmp_guid)
		{
			current = Process::Read<unsigned>(current + WowOffsets2::Player2::PlayerNameCacheNext);
			Process::ReadRaw(current + WowOffsets2::Player2::PlayerNameCacheGuid, &tmp_guid, 16);
		}
		name = Process::ReadString_UTF8(current + WowOffsets2::Player2::PlayerNameCacheName, 0);

	}
	return name;
}
