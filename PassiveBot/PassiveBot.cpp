// PassiveBot.cpp: определяет точку входа для консольного приложения.
//




#include "stdafx.h"
#include <iostream>
#include "AutoItX3_DLL.h"
//#include "boost\thread\thread.hpp"
#include <thread>
#include <random>
#include "Graphics\WorldViewer.h"
using namespace std;
using namespace Wow;
using namespace Utils;
using namespace Tools;

void init_static()
{
	Utils::Geometry::Transformer3D::Transformer3D();
	Game::LocationBase::Init();
	NavArea::InitNavConfig();
}




int main(int argc, wchar_t * argv[])
{

	init_static();
	setlocale(LC_ALL, "Russian");
	Sleep(5000);



	BotInteractor::StartGame("lissek7@ya.ru", "lebmat2762066", L"Тестируем");
	/*Area * area=new Area(Game::LocationBase::Get("Kalimdor"), Point2D<int>(33, 41), Point2D<int>(10, 3), 3);
	area->FindPath(Vector3(21882.5879, 22.1627789, -17879.6426), Vector3(22094.2168, 0.594630361, -17916.1914),0);*/

	
	Player * player = ObjectManager::GetPlayer();
	default_random_engine generator;
	uniform_int_distribution<int> distribution(0, ObjectManager::GetUnitsList().size() - 1);
	
	BotInteractor::GoToPoint(*ObjectManager::FindUnitByName(L"Тал"));

	//WorldViewer viewer=WorldViewer(Game::LocationBase::Get("Kalimdor"), Utils::WorldPositionToBlockCoords(player->GetPosition().coords), Utils::WorldPositionToChunkCoords(player->GetPosition().coords), 3);
	//viewer.ShowMap();

	while (1)
	{
		try
		{
			//BotInteractor::PulseCheck();
			//viewer.Update(Game::LocationBase::Get("Kalimdor"), Utils::WorldPositionToBlockCoords(player->GetPosition().coords), Utils::WorldPositionToChunkCoords(player->GetPosition().coords));
			Sleep(1000);
		}
		catch (...)
		{

		}
	}


	/*while (10)
	{
		player->DumpPosition();
		int random_variable = distribution(generator);
		Unit * unit = (*ObjectManager::GetUnitsList())[random_variable];
		wcout << unit->GetName() << endl;;
		BotInteractor::GoToPoint(*unit);
		///BotInteractor::PulseCheck();
		Sleep(1000);
	}
*/

	//_CrtDumpMemoryLeaks();
	return 0;

}
void DestructStatic()
{
	Utils::Geometry::Transformer3D::Transformer3D();
	Game::LocationBase::Clear();
	NavArea::InitNavConfig();
	ADTWorker::Clear();
	ChunkModel::Clear();
	ObjectManager::ClearAllLists();
	FrameManager::ClearFrames();
}
void foo()
{
	init_static();
	NavArea area = NavArea(Game::LocationBase::Get("Kalimdor"), Point2D<int>(34, 31/*28, 40*/), Point2D<int>(2, 16), 3);
	for (int i = 0; i < 1000; i++)
	{
		area.CheckAndUpdate(Game::LocationBase::Get("Kalimdor"), Point2D<int>(34, 31/*28, 40*/), Point2D<int>(7, 16));
		area.CheckAndUpdate(Game::LocationBase::Get("Kalimdor"), Point2D<int>(34, 31/*28, 40*/), Point2D<int>(2, 16));
		cout << i<<endl;
	}
	
	DestructStatic();
}

int main2(int argc, wchar_t * argv[])
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(301329);

	//main2(argc, argv);
	foo();

	
	//_CrtDumpMemoryLeaks();
	return 0;
}

