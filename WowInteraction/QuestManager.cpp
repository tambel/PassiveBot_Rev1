#include "stdafx.h"
#include "QuestManager.h"
using namespace Tools;

vector<unsigned> QuestManager::quest_ids;

QuestManager::QuestManager()
{
}


QuestManager::~QuestManager()
{
}

unsigned QuestManager::GetActiveQuestNumber()
{
	return Process::ReadRel<unsigned>(WowOffsets::Quest::QuestNumber);
}

void QuestManager::EnumActiveQuests()
{
	quest_ids.clear();
	for (int i = 0; i < (signed int)Process::ReadRel<unsigned>(WowOffsets::Quest::ExtendedQuestNumber); i++)
	{
		if (!(i < 0 || i >= (signed int)Process::ReadRel<unsigned>(WowOffsets::Quest::ExtendedQuestNumber) || Process::ReadRel<char>(WowOffsets::Quest::UnknonByteList + 16 * i) & 1))
		{
			quest_ids.push_back(Process::ReadRel<unsigned>(WowOffsets::Quest::QuestIdList + 16 * i));
		}

			//cout << Process::ReadRel<unsigned>(WowOffsets::Quest::QuestIdList + 16 * i) << endl;
	}
}

Quest QuestManager::GetQuest(unsigned id)
{
	//if (!id) return;
	unsigned result = 0;
	auto sub_1 = [](unsigned quest_cache, unsigned id)
	{
		auto sub_2 = [](unsigned quest_cache1,unsigned id)
		{
			auto sub_3 = [](unsigned id, unsigned digit_count = 4)
			{
				unsigned char   * id_ptr = reinterpret_cast<unsigned char*>(&id);
				unsigned result = -2128831035;
				while (digit_count)
				{
					--digit_count;
					
					result = 16777619 * (*id_ptr++ ^ result);
				}
				return result;
			};
			if (Process::Read<unsigned>(quest_cache1 + 36) != -1)
			{
				unsigned v3=sub_3(id);
				unsigned v5 = Process::Read<unsigned>(quest_cache1 + 24) + 12 * ((unsigned int)v3 % Process::Read<unsigned>(quest_cache1 + 36));
				unsigned result = Process::Read<unsigned>(v5 + 8);
				if (result & 1 || !result)
					result = 0;
				while (!(result & 1) && result)
				{
					if (Process::Read<unsigned>(result + 20) == v3 && Process::Read<unsigned>(result + 24) == id)
						return result;
					result = Process::Read<unsigned>(Process::Read<unsigned>(v5) + result + 4);
				}
			}
			
			return 0U;
		};
		unsigned edi = quest_cache + 0xC;
		unsigned result = sub_2(quest_cache + 0xC, id);

		return result;
	};
	unsigned v5 = Process::GetProcessBase() + WowOffsets::QuestCache;
	unsigned v7=sub_1(v5, id);
	if ( Process::Read<unsigned char>(v7 + 12092) & 1)
	{
		result = v7 + 28;
	}
	if (result)
	{
		return Process::Read<Quest>(result);
	}
}
