#include "stdafx.h"
using namespace Tools;
//using namespace WowOffsets;

unsigned ObjectManager::base;
vector<GameObject*> ObjectManager::game_objects = vector<GameObject*>();
vector<Item*> ObjectManager::items = vector<Item*>();
vector<Unit*> ObjectManager::units = vector<Unit*>();
vector<Player*> ObjectManager::players = vector<Player*>();
vector<WowObject*> ObjectManager::all_objects = vector<WowObject*>();
void ObjectManager::EnumAllVisibleObjects()
{
	ClearAllLists();
	unsigned next = Process::Read<unsigned>(base + WowOffsets2::ObjectManager2::NextObject);
	unsigned curobject = Process::Read<unsigned>(base + WowOffsets2::ObjectManager2::FirstObject);
	int count = 0;
	while (curobject != 0)
	{
		try
		{
			WowObject * object=nullptr;
			
			switch (WowObject::GetType_Static(curobject))
			{

			case ObjectType::GAMEOBJECT:
			{
				object = new GameObject(curobject);
				game_objects.push_back(static_cast<GameObject*>(object));
				//game_objects.push_back(new GameObject(curobject));
				break;
			}
			case ObjectType::ITEM:
			{
				object = new Item(curobject);
				items.push_back(static_cast<Item*>(object));
				//items.push_back(new Item(curobject));
				break;
			}
			case ObjectType::UNIT:
			{
				object = new Unit(curobject);
				units.push_back(static_cast<Unit*>(object));
				//units.push_back(new Unit(curobject));
				break;

			}
			case ObjectType::PLAYER:
			{
				object = new Player(curobject);
				players.push_back(static_cast<Player*>(object));
				//players.push_back(new Player(curobject));
				break;
			}

			}
			if (object)
				all_objects.push_back(object);
			

		}
		catch (MemoryReadException e)
		{
			break;
		}
		curobject = Process::Read<unsigned>(curobject + next +4);
		count++;
	}

}
void ObjectManager::Initialize()
{
	base = Process::ReadRel<unsigned>(WowOffsets2::ObjectManager2::ObjectManager);
	//base = Process::Read<unsigned>(Process::ReadRel<unsigned>(WowOffsets::ObjectManager::ObjectManagerPtr) + WowOffsets::ObjectManager::ObjectManagerOffset);
}

vector<GameObject*> &ObjectManager::GetGameObjectsList()
{
	return game_objects;
}
vector<Item*> &ObjectManager::GetItemsList()
{
	return items;
}
vector<Unit*> &ObjectManager::GetUnitsList()
{
	return units;
}
vector<Player*> &ObjectManager::GetPlayersList()
{
	return players;
}
void ObjectManager::ClearAllLists()
{
	for (auto object : all_objects)
	{
		delete object;
		object = nullptr;
	}
	for (auto game_object : game_objects)
	{
		delete game_object;
	}
	for (auto item : items)
	{
		delete item;
	}
	int i = 0;
	for (auto unit : units)
	{
		delete unit;
	}
	for (auto player : players)
	{
		delete player;
	}
	all_objects.clear();
	players.clear();
	game_objects.clear();
	items.clear();
	units.clear();
}
WowObject * ObjectManager::GetTargetObject()
{
	//EnumAllVisibleObjects();
	Guid128 target_guid = Process::ReadRel<Guid128>(WowOffsets::ObjectManager::TargetGUID);
	for (auto object : all_objects)
	{
		if (*object->GetGuid() == target_guid)
		{
			return object;
		}
	}
	return nullptr;
}
void ObjectManager::DumpAllObjectNames()
{
	cout << "Game Objects" << endl;
	for (auto game_object : game_objects)
	{
		wcout << game_object->GetName() << endl;
	}
	cout << "Items" << endl;
	for (auto item : items)
	{
		wcout << item->GetName() << endl;
	}
	cout << "Units" << endl;
	for (auto unit : units)
	{
		wcout << unit->GetName() << endl;
	}
	cout << "Players" << endl;
	for (auto player : players)
	{
		//player->GetName();
		wcout << player->GetName() << endl;
	}
}
Player * ObjectManager::GetPlayer()
{
	for (auto player : players)
	{
		if (player->GetBase() == Process::ReadRel<unsigned>(WowOffsets2::ObjectManager2::ActivePlayer))
		{
			return player;
		}
	}
	return 0;
}
Unit * ObjectManager::FindUnitByName(const wstring & name)
{
	for (auto unit : units)
	{
		if (unit->GetName() == name)
		{
			return unit;
		}
	}
	return 0;
}

