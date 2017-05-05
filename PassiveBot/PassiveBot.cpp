// PassiveBot.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include "WowWorldsData\WowWorldsData.h"

using namespace std;
using namespace Utils;
using namespace Tools;

#define BOTDEV

int bot_activity()
{

	ObjectManager::EnumAllVisibleObjects();
	Player * player = ObjectManager::GetPlayer();
	//shared_ptr<RemoteControl> remote_control= make_shared<RemoteControl>();
	shared_ptr<ViewerCommunication> viewer = make_shared<ViewerCommunication>();
	//remote_control->Start();
	viewer->Start();
	
	while (1)
	{
		player->DumpPosition();
		Sleep(100);
		//Unit * p = reinterpret_cast<Unit*>(ObjectManager::GetTargetObject());
		//if (p)
			//wcout<<p->GetName()<<endl;
		//BotInteractor::GoToPoint(*p);
		//remote_control->ProcessRequest();
		viewer->ProcessRequest();
		Sleep(1);

	}
	for (auto u : ObjectManager::GetUnitsList())
	{

		u->DumpPosition();
		wcout << u->GetName() << endl;
		Sleep(5000);
		BotInteractor::GoToPoint(*u);
	}
	
}

void test_db()
{
	auto areas=move(ClientDB::ReadWorldMapAreas());

	areas = areas;

}



int main(int argc, wchar_t * argv[])
{
	Sleep(5000);
	//test_db();
	BotInteractor::StartGame("arttambel@gmail.com", "archi911", L"Testintauenr", bot_activity);
	return 0;

}

