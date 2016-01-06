// PassiveBot.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include "AutoItX3_DLL.h"
//#include "boost\thread\thread.hpp"
#include <thread>
using namespace std;
using namespace Wow;
using namespace Utils;
using namespace Tools;

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
	Sleep(5000);

	

	BotInteractor::StartGame("lissek7@ya.ru", "lebmat2762066", L"Тестируем");

	
	Unit * unit = ObjectManager::FindUnitByName(L"Каут");
	BotInteractor::GetArea()->AddWowObjectAvatar(unit);
	//MapFrame * frame = new MapFrame();
	///frame->SetArea(BotInteractor::GetArea());
	//thread thread(workerFunc, frame, ObjectManager::GetPlayer()->GetPosition());
	//thread.detach();


	
	BotInteractor::FindPlayerPath(unit->GetPosition().coords);
	Player * player = ObjectManager::GetPlayer();
	//BotInteractor::GoThroughPath();
	//GameManager::RotatePlayer(0);
	float pos=0;
	float pos2 = 0;
	float old_pos=0;
	int sec = 0;
	int counter = 0;
	cout << "GO" << endl;
	vector<float> r = vector<float>();
	RECT rect = RECT();
	BotInteractor::FindPlayerPath(unit->GetPosition().coords);
	BotInteractor::GoThroughPath();
	//GameManager::GoToPoint(unit->GetPosition().coords);
	cin >> pos;
	while (1)
	{
		cout << GameManager::GetDistanceToPoint(player->GetPosition().coords, unit->GetPosition().coords)<<endl;
		float or = GameManager::GetOrientationToTarget(unit->GetPosition().coords);
		Sleep(100);
	}
	
	
	//cin >> pos;
	GameManager::RotatePlayer(unit);
	GameManager::GoToPoint(unit->GetPosition().coords);
	cin >> pos;

	return 0;

}

