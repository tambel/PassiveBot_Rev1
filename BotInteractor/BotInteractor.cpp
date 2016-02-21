#include "stdafx.h"
#include "BotInteractor.h"

using namespace std;
using namespace Wow;

NavArea BotInteractor::area;
bool BotInteractor::FindPath(Vector3 & start, Vector3 & end)
{
	Vector3 coords = start;
	coords = Vector3(Metrics::MapMidPoint - start.y, -(Metrics::MapMidPoint - start.x), start.z);
	//coords = coords - area.GetBoundingBox().up;
	Vector3 ucoords = end;
	ucoords = Vector3(Metrics::MapMidPoint - end.y, -(Metrics::MapMidPoint - end.x), end.z);
	//ucoords = ucoords - area.GetBoundingBox().up;
	int r = area.FindPath(Vector3(coords.x, coords.z, coords.y), Vector3(ucoords.x, ucoords.z, ucoords.y), 0);
	cout << r<<endl;
	//if (area.FindPath(Vector3(coords.x, coords.z, coords.y), Vector3(ucoords.x, ucoords.z, ucoords.y), 0) < 0)
	if (r<0)
	{
		return false;
	}
	
 //	PATHDATA * m_PathStore = area.m_PathStore.get();
	//int nVertCount = m_PathStore->MaxVertex;
	///*for (int nVert = 0; nVert < nVertCount; nVert++)
	//{
	//	m_PathStore->PosX[nVert] += area.GetBoundingBox().up.x;
	//	m_PathStore->PosY[nVert] += area.GetBoundingBox().up.z;
	//	m_PathStore->PosZ[nVert] += area.GetBoundingBox().up.y;
	//}*/
	area.to_update = true;
}
void BotInteractor::PulseCheck()
{
	Player * player = ObjectManager::GetPlayer();
	area.CheckAndUpdate(Game::LocationBase::Get("Kalimdor"), Utils::WorldPositionToBlockCoords(player->GetPosition().coords), Utils::WorldPositionToChunkCoords(player->GetPosition().coords));
}
void BotInteractor::__Test(int o)
{
	//AU3_Send(L"{NUMLOCK}");
	//AU3_MouseDown(L"Right");
	Sleep(100);
	RECT rect = RECT();
	
	AU3_WinGetClientSize(L"World of Warcraft", L"", &rect);
	for (int i = 0; i < 1; i++)
	{
		//cout << rect.right << endl;
		//AU3_MouseMove(rect.right / 2+o, rect.bottom / 2,1);
		AU3_MouseMove(rect.right/2 + o, rect.bottom / 2, 1);
		
		Sleep(1);
	}
	
	Sleep(100);
	
	//AU3_MouseUp(L"Right");
	//AU3_Send(L"{NUMLOCK}");
}
void BotInteractor::__Test2()
{
	Player * player = ObjectManager::GetPlayer();
	string path = "C:\\Users\\laptop\\Desktop\\pathing test\\tests\\";
	string test_name;
	struct Info {
		float srot;
		float erot;
		int offset;
	};
	ofstream file;
	vector<Info> infos = vector<Info>();
	int start = 600;
	RECT rect = RECT();

	AU3_WinGetClientSize(L"World of Warcraft", L"", &rect);
	AU3_MouseMove(0, rect.bottom / 2, 1);
	AU3_MouseDown(L"Right");
	
	for (int i = start; i <start + 83; i++)
	{
		Position pos = player->GetPosition();

		//file.open(path + to_string(i) + ".txt");
		///file << pos.rotation.z;
		Info info;
		info.srot = pos.rotation.z;
		info.offset = i;
		BotInteractor::__Test(i);
		info.erot = player->GetPosition().rotation.z;
		infos.push_back(info);
		
	}
	Sleep(1000);
	AU3_MouseUp(L"Right");
	file.open("C:\\Users\\laptop\\Desktop\\pathing test\\zdstest" + to_string(start) + ".txt");
	for (auto i : infos)
	{
		file << i.offset << " " << i.srot << " " << i.erot << endl;
	}
	file.close();
}
void BotInteractor::RotTest()
{
//	Player * player = ObjectManager::GetPlayer();
//	for (auto u : *ObjectManager::GetUnitsList())
//	{
//		AU3_MouseDown(L"Right");
//		GameManager::RotatePlayerTest(u->GetPosition().coords);
//		Sleep(1000);
//		AU3_MouseUp(L"Right");
//		Sleep(1000);
//		//GameManager::WorldToScreen(u->GetPosition().coords);
//
//	}
	Unit * unit = ObjectManager::FindUnitByName(L"июўщ");
	/*while (1)
	{
		cout << "Player " << player->GetPosition().rotation.z << " Targer: " << GameManager::GetOrientationToTarget(unit->GetPosition().coords)<<endl;
	}*/
	AU3_MouseDown(L"Right");
	Sleep(1000);
	GameManager::RotatePlayerTest(unit->GetPosition().coords);
	Sleep(1000);
	AU3_MouseUp(L"Right");
}
bool BotInteractor::FindPlayerPath(Vector3 & end)
{
	//area.Navigation().SetArea(area);
	return FindPath(ObjectManager::GetPlayer()->GetPosition().coords, end);
}
void BotInteractor::GoThroughPath()
{
	Player * player = ObjectManager::GetPlayer();
	Vector3 pos = player->GetPosition().coords;
	for (unsigned i = 0; i < area.m_nsmoothPath*3; i += 3)
	{
		Vector3 point=Vector3(Metrics::MapMidPoint + area.m_smoothPath[i + 2], Metrics::MapMidPoint - area.m_smoothPath[i], area.m_smoothPath[i + 1]);
		if (GameManager::GetPlayerDistanceToPoint(point) < 0.5 ) continue;
		GameManager::GoToPoint(point);
	}
	for (unsigned i = 0; i < area.m_nsmoothPath*3; i += 3)
	{
		//GameManager::GoToPoint(Vector3(area.m_smoothPath[i], area.m_smoothPath[i+1], area.m_smoothPath[i+2]));
	}


	//PATHDATA * abs_path= area.m_PathStore.get();
	//PATHDATA path;

	//int nVertCount = abs_path[0].MaxVertex;
	//Player * player = ObjectManager::GetPlayer();
	//Position pos = player->GetPosition();
	//for (int nVert = 0; nVert < nVertCount; nVert++)
	//{
	//	path.PosX[nVert] = Metrics::MapMidPoint + abs_path[0].PosZ[nVert];
	//	path.PosY[nVert] = Metrics::MapMidPoint-abs_path[0].PosX[nVert];
	//	path.PosZ[nVert] = abs_path[0].PosY[nVert];
	//}
	//for (int nVert = 0; nVert < nVertCount; nVert++)
	//{
	//	GameManager::GoToPoint(Vector3(path.PosX[nVert], path.PosY[nVert], path.PosZ[nVert]));
	//}
	////	GameManager::GoToPoint(Vector3(path.PosX[nVert], path.PosY[nVert], path.PosZ[nVert]));
}
void BotInteractor::GoThroughPathTest()
{
	Vector3 point;
	RECT rect = RECT();
	AU3_WinGetClientSize(L"World of Warcraft", L"", &rect);
	AU3_MouseMove(rect.right / 2, rect.bottom / 2);
	AU3_Send(L"{NUMLOCK}");
	AU3_MouseDown(L"Right");
	Sleep(100);
	for (unsigned i = 0; i < area.m_nsmoothPath * 3; i += 3)
	{
		point = Vector3(Metrics::MapMidPoint + area.m_smoothPath[i + 2], Metrics::MapMidPoint - area.m_smoothPath[i], area.m_smoothPath[i + 1]);
		if (GameManager::GetPlayerDistanceToPoint(point) < 2.5) continue;
		GameManager::RotatePlayerTest(point);
		while (GameManager::GetPlayerDistanceToPoint(point)>3.0f)
		{
			Sleep(1);
		}
		cout << "111111"<<endl;
	}
	
	//AU3_Send(L"{Up up}");
	
	AU3_MouseUp(L"Right");
	AU3_Send(L"{NUMLOCK}");
}
void BotInteractor::StartGame(string login, string password,wstring char_name)
{

	GameStartParam  param = GameStartParam();
	param.char_name = char_name;
	param.login = login;
	param.password = password;
	GameInteractor::Start(&param);
	ObjectManager::Initialize();
	ObjectManager::EnumAllVisibleObjects();
	Player * player = ObjectManager::GetPlayer();
	//Area tmp_area=Area(Game::LocationBase::Get("Kalimdor"), Utils::WorldPositionToBlockCoords(player->GetPosition().coords), Utils::WorldPositionToChunkCoords(player->GetPosition().coords), 2);
	//area = move(tmp_area);
	//area=move(NavArea(Game::LocationBase::Get("Kalimdor"), Utils::WorldPositionToBlockCoords(player->GetPosition().coords), Utils::WorldPositionToChunkCoords(player->GetPosition().coords), 3));
	
	///area.AddWowObjectAvatar(player);

}

void BotInteractor::GoToPoint(Vector3 & point)
{
	bool r = FindPlayerPath(point);
	cout << r << endl;
	if (!FindPlayerPath(point))
	{
		AU3_Send(L"{SPACE down}");
		return;
	}
	//GoThroughPath();
	GoThroughPathTest();

}

