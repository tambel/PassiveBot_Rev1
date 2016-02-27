#pragma once
#include "WowInteraction\Player.h"
using namespace Wow;
class BotInteractor
{
	static NavArea area;
	static bool FindPath(Vector3 & start, Vector3 & end);
	
	
public:
	static void GoThroughPath(); 
	static void StartGame(string login,string password,wstring char_name);
	static void GoToPoint(Vector3 & point);
	inline static void GoToPoint(WowObject & object) { GoToPoint(object.GetPosition().coords); }
	inline static Area  * GetArea() { return &area; }
	static bool FindPlayerPath(Vector3 & end);
	static void PulseCheck();
};

