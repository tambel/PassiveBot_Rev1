#include "stdafx.h"
#include "BotInteractor.h"

using namespace std;

NavArea BotInteractor::area;
bool BotInteractor::FindPath(Vector3 & start, Vector3 & end)
{
	Vector3 coords = start;
	coords = Vector3(Metrics::MapMidPoint - start.y, -(Metrics::MapMidPoint - start.x), start.z);
	Vector3 ucoords = end;
	ucoords = Vector3(Metrics::MapMidPoint - end.y, -(Metrics::MapMidPoint - end.x), end.z);
	//int r = area.FindPath(Vector3(coords.x, coords.z, coords.y), Vector3(ucoords.x, ucoords.z, ucoords.y), 0);

	if (area.FindPath(Vector3(coords.x, coords.z, coords.y), Vector3(ucoords.x, ucoords.z, ucoords.y), 0)<0)
	{
		return false;
	}
	return true;
}
void BotInteractor::PulseCheck()
{
		Player * player = ObjectManager::GetPlayer();
		area.CheckAndUpdate(Game::LocationBase::Get("Kalimdor"), Utils::WorldPositionToBlockCoords(player->GetPosition().coords), Utils::WorldPositionToChunkCoords(player->GetPosition().coords));
}
void BotInteractor::Init()
{
#ifdef DEBUG
	setlocale(LC_ALL, "Russian");
#endif // DEBUG

	Utils::Geometry::Transformer3D::Transformer3D();
	Game::LocationBase::Init();
	NavArea::InitNavConfig();
}
void BotInteractor::CleanUp()
{
}

bool BotInteractor::FindPlayerPath(Vector3 & end)
{
	return FindPath(ObjectManager::GetPlayer()->GetPosition().coords, end);
}
void BotInteractor::GoThroughPath()
{
	Vector3 point;
	RECT rect = RECT();
	AU3_WinGetClientSize(L"World of Warcraft", L"", &rect);
	AU3_MouseMove(rect.right / 2, rect.bottom / 2);
	AU3_Send(L"{NUMLOCK}");
	AU3_MouseDown(L"Right");
	Sleep(100);
	for (int i = 0; i < area.m_nsmoothPath * 3; i += 3)
	{
		point = Vector3(Metrics::MapMidPoint + area.m_smoothPath[i + 2], Metrics::MapMidPoint - area.m_smoothPath[i], area.m_smoothPath[i + 1]);
		/*if (GameManager::GetPlayerDistanceToPoint(point) < 2.5) continue;
		GameManager::RotatePlayerTest(point);
		while (GameManager::GetPlayerDistanceToPoint(point)>3.0f)*/
		if (GameManager::GetPlayerDistanceToPoint(point) < 4.0) continue;
		GameManager::RotatePlayer(point);
		while (GameManager::GetPlayerDistanceToPoint(point)>4.0f)
		{
			Sleep(1);
		}
	}
	
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
	area=NavArea(3);

}

void BotInteractor::GoToPoint(Vector3 & point)
{
	if (area.IsOutOfBounds(LocationBase::Get("Kalimdor"), Utils::WorldPositionToBlockCoords(point), Utils::WorldPositionToChunkCoords(point)))
	{
		Player * player = ObjectManager::GetPlayer();
		area.UpdateArea(LocationBase::Get("Kalimdor"), Utils::WorldPositionToBlockCoords(player->GetPosition().coords), Utils::WorldPositionToChunkCoords(player->GetPosition().coords));
		cout << "UPDATED!!!!!!!" << endl;
		cout << "UPDATED!!!!!!!" << endl;
		cout << "UPDATED!!!!!!!" << endl;
	}
	if (!FindPlayerPath(point))
	{
		AU3_Send(L"{SPACE down}");
		return;
	}
	GoThroughPath();


}

