#include "stdafx.h"
using namespace Tools;
//using namespace WowOffsets;
namespace Wow
{
	unsigned ObjectManager::base;
	vector<GameObject*> * ObjectManager::game_objects=new vector<GameObject*>();
	vector<Item*> * ObjectManager::items=new vector<Item*>();
	vector<Unit*> * ObjectManager::units=new vector<Unit*>();
	vector<Player*> * ObjectManager::players=new vector<Player*>();
	void ObjectManager::EnumAllVisibleObjects()
	{
		ClearAllLists();
		unsigned curobject=Process::ReadUInt(base+WowOffsets::ObjectManager::FirstObject);
		int count=0;
		while (curobject!=0)
		{
			switch (WowObject::GetType_Static(curobject))
			{
			case ObjectType::GAMEOBJECT:
				{
					game_objects->push_back(new GameObject(curobject));
					break;
				}
			case ObjectType::ITEM:
				{
					items->push_back(new Item(curobject));
					break;
				}
			case ObjectType::UNIT:
				{
					units->push_back(new Unit(curobject));
					break;

				}
			case ObjectType::PLAYER:
				{
					players->push_back(new Player(curobject));
					break;
				}
			}

			curobject=Process::ReadUInt(curobject+WowOffsets::ObjectManager::NextObject);

		}
	}
	void ObjectManager::Initialize()
	{

		base=Process::ReadUInt(Process::ReadRelUInt(WowOffsets::ObjectManager::ObjectPanagerPtr)+WowOffsets::ObjectManager::ObjectManagerOffset);
	}

	vector<GameObject*> *ObjectManager::GetGameObjectsList()
	{
		return game_objects;
	}
	vector<Item*> *ObjectManager::GetItemsList()
	{
		return items;
	}
	vector<Unit*> *ObjectManager::GetUnitsList()
	{
		return units;
	}
	vector<Player*> *ObjectManager::GetPlayersList()
	{
		return players;
	}
	void ObjectManager::ClearAllLists()
	{
		for(auto game_object:*game_objects)
		{
			delete game_object;
		}
		for(auto item:*items)
		{
			delete item;
		}
		int i=0;
		for (auto unit:*units)
		{
			delete unit;
		}
		for (auto player:*players)
		{
			delete player;
		}
		players->clear();
		game_objects->clear();
		items->clear();
		units->clear();
	}
	void ObjectManager::DumpAllObjectNames()
	{
		cout<<"Game Objects"<<endl;
		for (auto game_object:*game_objects)
		{
			wcout<<game_object->GetName()<<endl;
		}
		cout<<"Items"<<endl;
		for (auto item:*items)
		{
			wcout<<item->GetName()<<endl;
		}
		cout<<"Units"<<endl;
		for (auto unit:*units)
		{
			wcout<<unit->GetName()<<endl;
		}
		cout<<"Players"<<endl;
		for (auto player:*players)
		{
			//player->GetName();
			wcout<<player->GetName()<<endl;
		}
	}
	Player * ObjectManager::GetPlayer()
	{
		for (auto player:*players)
		{
			Guid128 tmp;
			Guid128 pl=*player->GetGuid();
			if(player->GetBase()==Process::ReadRelUInt(WowOffsets::ObjectManager::LocalPlayer))
			{
				return player;
			}
		}
		return 0;
	}
	Unit * ObjectManager::FindUnitByName(wchar_t * name)
	{
		for (auto unit:*units)
		{
			if (wcscmp(name,unit->GetName())==0)
			{
				return unit;
			}
		}
		return 0;
	}
}
