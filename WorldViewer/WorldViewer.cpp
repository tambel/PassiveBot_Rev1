// WorldViewer.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include "ViewerClient.h"

using namespace std;


void init_static()
{
	Utils::Geometry::Transformer3D::Transformer3D();
	Game::LocationBase::Init();
	NavArea::InitNavConfig();
}
int main(int argc, char* argv[])
{
	init_static();
	ViewerClient client = ViewerClient();
	client.Connect();
	
	Generator gen = Generator();
	gen.Generate(WorldPositionToBlockCoords(client.GetPlayerPosition().coords), WorldPositionToChunkCoords(client.GetPlayerPosition().coords));
	WorldViewer viewer = WorldViewer(client.GetPlayerPosition().coords);
	viewer.ShowMap();
	while (1)
	{
		//viewer.Update(client.GetPlayerPosition().coords);
		Sleep(100);
	}
    return 0;
}

int main2()
{
	init_static();
	WorldViewer viewer = WorldViewer(LocationBase::Get("Kalimdor"), Point2DI(36, 32), Point2DI(7, 5),1);
	viewer.ShowMap();
	while (1)
	{
		//viewer.Update(Point2DI(36, 32), Point2DI(7, 4));
		Sleep(100);
	}
	return 0;
}

