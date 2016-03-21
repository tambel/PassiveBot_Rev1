// PassiveBot.cpp: определяет точку входа для консольного приложения.
//




#include "stdafx.h"
#include <iostream>
#include "AutoItX3_DLL.h"
#include <thread>
#include <random>
#include "WowInteraction\DBCReader.h"

#include <fstream>


#include "NetworkCommunication\NetworkCommunicatorServer.h"

using namespace std;
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
	WoWClientDB bd;




	//NetworkCommunicatorServer server = NetworkCommunicatorServer();

	Player * p = ObjectManager::GetPlayer();

	BotInteractor::GoToPoint(*ObjectManager::GetTargetObject());
	while (1)
	{

		p->DumpPosition();
		Sleep(500);
	}

	

	//BotInteractor::GoToPoint(*ObjectManager::FindUnitByName(L"Так"));
	QuestManager::EnumActiveQuests();
	FrameManager::InitKnownFrames();

	
	


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
	NavArea area = NavArea(3);
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

