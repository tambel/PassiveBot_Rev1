// PassiveBot.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include "boost\thread\thread.hpp"
using namespace std;
using namespace Wow;
using namespace Utils;

void workerFunc()  
{  
	
	Window * win=new Window();
	win->go();

}

int main(int argc, wchar_t * argv[])
{
	setlocale( LC_ALL,"Russian" );


	boost::thread thread(workerFunc);
	thread.detach();
	thread.join();

	
	Sleep(5000);

	GameStartParam  param= GameStartParam();
	param.char_name=L"Люблюдашу";
	param.login="lissek7@ya.ru";
	param.password="lebmat2762066";

	bool y=GameInteractor::Start(&param);
	
	ObjectManager::Initialize();
	ObjectManager::EnumAllVisibleObjects();
	Player * player = ObjectManager::GetPlayer();
	Vector3 pos=player->GetPosition().coords;
	player->DumpPosition();
	
	while (1)
	{
		player->DumpPosition();
		Sleep(100);
	}
	

}

