#include "stdafx.h"
#include "BotInteractor.h"

using namespace std;
using namespace Wow;

SquareArea BotInteractor::area;
bool BotInteractor::FindPath(Vector3 & start, Vector3 & end)
{
	Vector3 coords = start;
	coords = Vector3(Metrics::MapMidPoint - start.y, -(Metrics::MapMidPoint - start.x), start.z);
	coords = coords - area.GetBoundingBox().up;
	Vector3 ucoords = end;
	ucoords = Vector3(Metrics::MapMidPoint - end.y, -(Metrics::MapMidPoint - end.x), end.z);
	ucoords = ucoords - area.GetBoundingBox().up;
	if (area.Navigation().FindPath(Vector3(coords.x, coords.z, coords.y), Vector3(ucoords.x, ucoords.z, ucoords.y), 0, 0) < 0)
	{
		return false;
	}
	int nVertCount = area.Navigation().m_PathStore[0].MaxVertex;
	for (int nVert = 0; nVert < nVertCount; nVert++)
	{
		area.Navigation().m_PathStore[0].PosX[nVert] += area.GetBoundingBox().up.x;
		area.Navigation().m_PathStore[0].PosY[nVert] += area.GetBoundingBox().up.z;
		area.Navigation().m_PathStore[0].PosZ[nVert] += area.GetBoundingBox().up.y;


	}
	area.to_update = true;
}
bool BotInteractor::FindPlayerPath(Vector3 & end)
{
	return FindPath(ObjectManager::GetPlayer()->GetPosition().coords, end);
}
void BotInteractor::GoThroughPath()
{
	PATHDATA * abs_path = area.Navigation().m_PathStore;
	PATHDATA path;

	int nVertCount = abs_path[0].MaxVertex;
	Player * player = ObjectManager::GetPlayer();
	Position pos = player->GetPosition();
	for (int nVert = 0; nVert < nVertCount; nVert++)
	{
		path.PosX[nVert] = Metrics::MapMidPoint + abs_path[0].PosZ[nVert];
		path.PosY[nVert] = Metrics::MapMidPoint-abs_path[0].PosX[nVert];
		path.PosZ[nVert] = abs_path[0].PosY[nVert];
	}
	for (int nVert = 0; nVert < nVertCount; nVert++)
	{
		GameManager::GoToPoint(Vector3(path.PosX[nVert], path.PosY[nVert], path.PosZ[nVert]));
	}
	//	GameManager::GoToPoint(Vector3(path.PosX[nVert], path.PosY[nVert], path.PosZ[nVert]));
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
	//SquareArea tmp_area=SquareArea(Game::LocationBase::Get("Kalimdor"), Utils::WorldPositionToBlockCoords(player->GetPosition().coords), Utils::WorldPositionToChunkCoords(player->GetPosition().coords), 2);
	//area = move(tmp_area);
	area=move(SquareArea(Game::LocationBase::Get("Kalimdor"), Utils::WorldPositionToBlockCoords(player->GetPosition().coords), Utils::WorldPositionToChunkCoords(player->GetPosition().coords), 2));
	area.AddWowObjectAvatar(player);

}

void BotInteractor::GoToPoint(Vector3 & point)
{

}

