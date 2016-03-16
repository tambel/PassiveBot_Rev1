#pragma once
#include "GameObject.h"
#include "WowObject.h"
#include "Item.h"
#include "Unit.h"
#include "Player.h"
#include <vector>
using namespace std;

class ObjectManager
{

	static unsigned base;
	static vector<GameObject*> game_objects;
	static vector<Item*>  items;
	static vector <Unit*>  units;
	static vector <Player*>  players;
	static vector <WowObject*>  all_objects;

public:
	static void EnumAllVisibleObjects();
	static void Initialize();
	static vector<GameObject*> & GetGameObjectsList();
	static vector<Item*> &GetItemsList();
	static vector<Unit*> &GetUnitsList();
	static vector<Player*> &GetPlayersList();
	static void DumpAllObjectNames();
	static Player * GetPlayer();
	static Unit * FindUnitByName(const wstring & name);
	static void ClearAllLists();
	static WowObject * GetTargetObject();
};



