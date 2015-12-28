// PassiveBot.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include "boost\thread\thread.hpp"
using namespace std;
using namespace Wow;
using namespace Utils;
using namespace Utils;
void workerFunc(MapFrame * frame,Position pos)  
{  
	
	frame->go();
	frame->SetCamera(pos);

}
void init_static()
{
	Utils::Geometry::Transformer3D::Transformer3D();
	Game::LocationBase::Init();
}

int main(int argc, wchar_t * argv[])
{
	init_static();
	setlocale( LC_ALL,"Russian" );
	MapFrame * frame2 = new MapFrame();
	//trool vilage 33,41
	//orgrimmar 28 40
	SquareArea * area2 = new SquareArea(Game::LocationBase::Get("Kalimdor"), Point2D<int>(28, 40), Point2D<int>(10, 3),5);
	area2->ToMesh();
	frame2->SetArea(area2);
	frame2->go();

	

	
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
	/*if (!Game::LocationBase::IsInitialized())
		Game::LocationBase::Init();*/

	MapFrame * frame=new MapFrame();
	SquareArea * area=new SquareArea(Game::LocationBase::Get("Kalimdor"),Utils::WorldPositionToBlockCoords(player->GetPosition().coords),Utils::WorldPositionToChunkCoords(player->GetPosition().coords),9);//Point2D<int>(0,1),10);
	area->AddWowObjectAvatar(player);
	frame->SetArea(area);

	boost::thread thread(workerFunc,frame,player->GetPosition());
	thread.detach();
	thread.join();
	while (1)
	{

		area->Move(Game::LocationBase::Get("Kalimdor"),Utils::WorldPositionToBlockCoords(player->GetPosition().coords),Utils::WorldPositionToChunkCoords(player->GetPosition().coords));

		//area=new SquareArea(Game::LocationBase::Get("Kalimdor"),Utils::WorldPositionToBlockCoords(player->GetPosition().coords),Utils::WorldPositionToChunkCoords(player->GetPosition().coords),10);//Point2D<int>(0,1),10);
		cout << "UPDATED" << endl;
		Sleep(500);
	}
	return 0;

}

