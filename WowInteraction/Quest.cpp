#include "Quest.h"
#include "stdafx.h"

using namespace Tools;
void Quest::GetObjectivesBlobs()
{
	QuestManager::SelectQuestInJournal(id);
	unsigned v5 = Process::ReadRel<unsigned>(WowOffsets::Quest::QuestBlobInfo1) + 0x26C;
	unsigned v9;
	for (int i = 0; i<25; i++)
	{
		v9 = Process::Read<unsigned>(v5 + 24 * i);
		if (!v9)
			continue;
		if (Process::Read<unsigned>(v9) == id)
		{
			break;
		}
	}
	if (Process::Read<unsigned>(v9) != id)
	{
		return;
	}
	unsigned point_count=0;
	unsigned v3 = 0;
	int * points;
	for (int i = 0; i < Process::Read<unsigned>(Process::Read<unsigned>(v5) + 4); v3 += 0x4C)
	{
		unsigned a1 = v3 + Process::Read<unsigned>(Process::Read<unsigned>(v5) + 8);
		if (a1)
		{
			if (Process::Read<unsigned>(a1) >= 3U)
			{
				point_count = Process::Read<unsigned>(a1);
				if (point_count)
				{
					points = new  int[point_count * 2];
					Process::ReadRaw(Process::Read<unsigned>(a1 + 4), points, point_count * 8);
					break;
				}
			}
		}
	}
	//QuestObjectiveArea area;
	//area.points_count = point_count;
	//area.points = reinterpret_cast<QuestAreaPoint*> (points);
	if (kill_objective1!=nullptr)
	{
		kill_objective1->area.points_count = point_count;
		kill_objective1->area.points = reinterpret_cast<QuestAreaPoint*> (points);
	}
}
bool Quest::Init(unsigned id)
{
	auto read_from_client = [](unsigned id)
	{
		unsigned result = 0;
		auto sub_1 = [](unsigned quest_cache, unsigned id)
		{
			auto sub_2 = [](unsigned quest_cache1, unsigned id)
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
					unsigned v3 = sub_3(id);
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
		unsigned v7 = sub_1(v5, id);
		if (Process::Read<unsigned char>(v7 + 12092) & 1)
		{
			result = v7 + 28;
		}
		return result;
	};

	unsigned result_addr = read_from_client(id);
	_Quest result = Process::Read<_Quest>(result_addr);
	id = result.id;
	if (result.Objective1)
	{
		_KillCreatureObjective _obj;
		try
		{
			_obj=Process::Read<_KillCreatureObjective>(result.Objective1);
		}
		catch (const MemoryReadException& e)
		{
			return 0;
		}
		objectives_count++;
		kill_objective1 = make_shared<QuestKillObjective>();
		kill_objective1->count = _obj.creatures_count;
		kill_objective1->unit_id = _obj.creature_id;
		//objectives[objectives_count - 1] = obj;
	}
}
Quest::Quest()
{
}
Quest::Quest(_Quest q)
{
	id = q.id;
	Init(id);
	if (kill_objective1)
		GetObjectivesBlobs();

}

Quest::Quest(unsigned id)
{
	if (!Init(id))
	{
		throw(exception());
	}
	GetObjectivesBlobs();
}

Quest::~Quest()
{
}
