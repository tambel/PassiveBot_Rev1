// PassiveBot.cpp: определяет точку входа для консольного приложения.
//




#include "stdafx.h"
#include <iostream>
#include "AutoItX3_DLL.h"
#include <thread>
#include <random>
#include "WowInteraction\DBCReader.h"

#include <fstream>


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
	//QuestManager::EnumActiveQuests();
	QuestManager::EnumActiveQuests();
	FrameManager::EnumAllFrames();
	//FrameManager::InitKnownFrames();

	Player * p = ObjectManager::GetPlayer();
	Position pos = p->GetPosition();
	Quest q = QuestManager::GetQuest(14456);
	for (int i = 0; i < q.kill_objective1->area.points_count; i++)
	{
		cout <<dec<< q.kill_objective1->area.points[i][0] << " " << q.kill_objective1->area.points[i][1] << endl;
	}
	for (auto id : QuestManager::GetQuestIds())
	{
		QuestManager::SelectQuestInJournal(id);
		Sleep(1000);
	}




 	FrameManager::EnumAllFrames();
	Frame * wm=FrameManager::FindFrameByName("QuestScrollFrame");
	Frame * pwm = wm->GetParent();
	vector<Frame*> wm_child;
	for (auto frame : FrameManager::GetFrames())
	{
		if (frame->GetParent() == wm)
		{
			frame->GetName();
			wm_child.push_back(frame);
		}
	}
	for (auto frame : wm_child)
	{
		
		frame->MoveMouseToFrame();
		Sleep(3000);
	}
	wm_child.back()->GetName();
	wm_child.back()->GetLabelText();
	wm_child.back()->MoveMouseToFrame();
	vector<Frame*> wm_child2;
	for (auto frame : FrameManager::GetFrames())
	{
		if (frame->GetParent() == wm_child.back())
		{
			frame->GetName();

			frame->GetLabelText();
			wm_child2.push_back(frame);
		}
	}
	Frame * f2 = wm_child2[3];
	vector<Frame*> wm_child3;
	for (auto frame : FrameManager::GetFrames())
	{
		if (frame->GetParent() == f2)
		{
			frame->GetName();

			frame->GetLabelText();
			wm_child3.push_back(frame);
		}
	}
	for (auto frame : wm_child3)
	{
		Sleep(3000);
		frame->MoveMouseToFrame();
		
	}



	QuestManager::EnumActiveQuests();
	for (auto qi : QuestManager::GetQuestIds())
	{
		cout << qi << endl;
		//QuestManager::GetQuestBlob(qi);
	}
	


	while (1)
	{

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

