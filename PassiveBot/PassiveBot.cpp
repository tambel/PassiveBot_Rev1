// PassiveBot.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include "boost\thread\thread.hpp"
using namespace std;
using namespace Wow;
using namespace Utils;

void workerFunc(MapFrame * frame,Position pos)  
{  
	
	frame->go();
	frame->SetCamera(pos);

}

int main(int argc, wchar_t * argv[])
{
	setlocale( LC_ALL,"Russian" );


	

	
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
	if (!Game::LocationBase::IsInitialized())
		Game::LocationBase::Init();

	MapFrame * frame=new MapFrame();
	SquareArea * area=new SquareArea(Game::LocationBase::Get("Kalimdor"),Utils::WorldPositionToBlockCoords(player->GetPosition().coords),Utils::WorldPositionToChunkCoords(player->GetPosition().coords),5);//Point2D<int>(0,1),10);
	area->AddWowObjectAvatar(player);
	frame->SetArea(area);

	boost::thread thread(workerFunc,frame,player->GetPosition());
	thread.detach();
	thread.join();
	while (1)
	{

		area->Move(Game::LocationBase::Get("Kalimdor"),Utils::WorldPositionToBlockCoords(player->GetPosition().coords),Utils::WorldPositionToChunkCoords(player->GetPosition().coords));

		// area=new SquareArea(Game::LocationBase::Get("Kalimdor"),Utils::WorldPositionToBlockCoords(player->GetPosition().coords),Utils::WorldPositionToChunkCoords(player->GetPosition().coords),10);//Point2D<int>(0,1),10);
	
		Sleep(500);
	}
	return 0;

}

