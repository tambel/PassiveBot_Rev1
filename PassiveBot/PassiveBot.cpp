// PassiveBot.cpp: определяет точку входа для консольного приложения.
//




#include "stdafx.h"
#include <iostream>
#include "AutoItX3_DLL.h"
#include <thread>
#include <random>
#include "WowInteraction\DBCReader.h"
using namespace std;
using namespace Utils;
using namespace Tools;

void init_static()
{
	Utils::Geometry::Transformer3D::Transformer3D();
	Game::LocationBase::Init();
	NavArea::InitNavConfig();
}



//struct WoWClientDB;
int main(int argc, wchar_t * argv[])
{

	init_static();

	setlocale(LC_ALL, "Russian");
	Sleep(5000);



	BotInteractor::StartGame("lissek7@ya.ru", "lebmat2762066", L"Тестируем");
	WoWClientDB bd;

	/*unsigned add = Process::ReadRel<unsigned>(0xd7bc28);
	wstring s = Process::ReadString_UTF8(add, 1000);*/

	Test t;
	t = Process::ReadRel<Test>(0xD7BF94);
	Player * player = ObjectManager::GetPlayer();
	

	//BotInteractor::GoToPoint(*ObjectManager::FindUnitByName(L"Так"));

	while (1)
	{
		try
		{
			cout << "------------------" << endl;
			QuestManager::EnumActiveQuests();
			for (auto qi : QuestManager::GetQuestIds())
			{
				Quest q = QuestManager::GetQuest(qi);
				wchar_t n[256];
				wchar_t o[1500];
				wchar_t d[1500];
				MultiByteToWideChar(65001, 0, (LPCCH)q.Name, -1, n, 512);
				MultiByteToWideChar(65001, 0, (LPCCH)q.ObjectiveText, -1, o, 3000);
				MultiByteToWideChar(65001, 0, (LPCCH)q.Description, -1, d, 3000);
				wcout << n << endl << o << endl << d << endl;;
			}
			cout << "------------------" << endl;
			//cout << QuestManager::GetActiveQuestNumber() << endl;


			//BotInteractor::PulseCheck();
			//GameManager::WorldToScreen(ObjectManager::FindUnitByName(L"Так")->GetPosition());
			Sleep(10);
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

