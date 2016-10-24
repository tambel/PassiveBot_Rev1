// PassiveBot.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include <iostream>
#include "AutoItX3_DLL.h"
#include <thread>
#include <random>
#include "WowInteraction\DBCReader.h"
#include <fstream>
#include "NetworkCommuniсation\NetworkCommunicatorServer.h"
using namespace std;
using namespace Utils;
using namespace Tools;


int bot_activity()
{

	M2 m2 = M2("Character\\Tauren\\Male\\TaurenMale.M2");
	ObjectManager::EnumAllVisibleObjects();
	Player * p = ObjectManager::GetPlayer();
	for (auto u:ObjectManager::GetUnitsList())
	{
		
		u->DumpPosition();
		wcout << u->GetName() <<endl;
		Sleep(5000);
		BotInteractor::GoToPoint(*u);
	}
	while (1)
	{
		p->DumpPosition();
		Sleep(50);
	}
	return 0;
}

int main(int argc, wchar_t * argv[])
{
	
	Sleep(5000);
	BotInteractor::StartGame("arttambel@gmail.com", "archi911", L"Testintauenr", bot_activity);
	return 0;

}

