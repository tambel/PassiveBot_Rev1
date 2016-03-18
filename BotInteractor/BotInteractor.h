#pragma once
#include "WowInteraction\Player.h"
#include "WowWorldsData\NavArea.h"

class BotInteractor
{
	static NavArea area;
	static bool FindPath(Vector3 & start, Vector3 & end);
	static bool player_is_moving;
	
	
public:
	static void GoThroughPath(); 
	static void StartGame(string login,string password,wstring char_name);
	static void GoToPoint(Vector3 & point);
	inline static void GoToPoint(WowObject & object) { GoToPoint(object.GetPosition().coords); }
	static Area  * GetArea() { return &area; }
	static bool FindPlayerPath(Vector3 & end);
	static void PulseCheck();
	static void Init();
	static void CleanUp();
};

