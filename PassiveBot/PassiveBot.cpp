// PassiveBot.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
//#include "boost\thread\thread.hpp"
#include <thread>
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
	
	//trool vilage 33,41
	//orgrimmar 28 40
	
	/*MapFrame * frame2 = new MapFrame();
	SquareArea * area2 = new SquareArea(Game::LocationBase::Get("Kalimdor"), Point2D<int>(34, 32), Point2D<int>(10, 3),9);
	area2->ToMesh();
	Navigation nav = Navigation();
	nav.SetArea(*area2);
	nav.BuildNavMesh();
	int k=nav.FindPath(Vector3(5.0f, 5.0f, -33.34f), Vector3(80.0f, 15.0f, 70.34f), 0, 0);
	cout << k << endl;
	frame2->m_PathStore = nav.m_PathStore;
	frame2->SetArea(area2);
	frame2->SetNavMesh(*nav.result_mesh);
	
	
	frame2->go();*/

	

	
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
	Unit * unit = ObjectManager::FindUnitByName(L"Мишики");
	MapFrame * frame=new MapFrame();
	SquareArea * area=new SquareArea(Game::LocationBase::Get("Kalimdor"),Utils::WorldPositionToBlockCoords(player->GetPosition().coords),Utils::WorldPositionToChunkCoords(player->GetPosition().coords),2);//Point2D<int>(0,1),10);
	area->AddWowObjectAvatar(player);
	area->AddWowObjectAvatar(unit);
	Vector3 coords = player->GetPosition().coords;
	coords = Vector3(Metrics::MapMidPoint - coords.y, -(Metrics::MapMidPoint - coords.x), coords.z);
	coords = coords - area->GetBoundingBox().up;
	Vector3 ucoords = unit->GetPosition().coords;
	ucoords = Vector3(Metrics::MapMidPoint - ucoords.y, -(Metrics::MapMidPoint - ucoords.x), ucoords.z);
	ucoords = ucoords - area->GetBoundingBox().up;
	int k;
	k=area->Navigation().FindPath(Vector3(coords.x, coords.z, coords.y), Vector3(ucoords.x, ucoords.z, ucoords.y), 0, 0);
	int nVertCount = area->Navigation().m_PathStore[0].MaxVertex;
	for (int nVert = 0; nVert < nVertCount; nVert++)
	{
		area->Navigation().m_PathStore[0].PosX[nVert]+= area->GetBoundingBox().up.x;
		area->Navigation().m_PathStore[0].PosY[nVert] += area->GetBoundingBox().up.z;
		area->Navigation().m_PathStore[0].PosZ[nVert] += area->GetBoundingBox().up.y;
			

	}
	cout << k << endl;
	//area->Navigation().FindPath(Vector3(5, 5, 30), Vector3(15, 15, 30), 0, 0);
	frame->SetArea(area);
	thread thread(workerFunc,frame,player->GetPosition());
	thread.detach(); 


	while (1)
	{

		area->Move(Game::LocationBase::Get("Kalimdor"),Utils::WorldPositionToBlockCoords(player->GetPosition().coords),Utils::WorldPositionToChunkCoords(player->GetPosition().coords));


		//area=new SquareArea(Game::LocationBase::Get("Kalimdor"),Utils::WorldPositionToBlockCoords(player->GetPosition().coords),Utils::WorldPositionToChunkCoords(player->GetPosition().coords),10);//Point2D<int>(0,1),10);
		cout << "UPDATED " <<k<< endl;
		Sleep(500);
	}
	return 0;

}

