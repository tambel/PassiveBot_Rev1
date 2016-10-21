#pragma once
#include "WowInteraction\Player.h"
#include "WowWorldsData\NavArea.h"

typedef int(*working_func)();

class BotInteractor
{
	static NavArea area;
	static bool FindPath(Vector3 & start, Vector3 & end);
	static bool player_is_moving;
	static working_func func;
	
	
public:
	static void GoThroughPath(); 
	static void StartGame(string login,string password,wstring char_name, int(*working_func)());
	static void GoToPoint(Vector3 & point);
	inline static void GoToPoint(WowObject & object) { GoToPoint(object.GetPosition().coords); }
	static Area  * GetArea() { return &area; }
	static bool FindPlayerPath(Vector3 & end);
	static void PulseCheck();
	static void Init();
	static void CleanUp();
	static int _Start();
};

