// WorldViewer.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include "Graphics\WorldViewer.h"
#include <iostream>
using namespace std;


void init_static()
{
	Utils::Geometry::Transformer3D::Transformer3D();
	Game::LocationBase::Init();
	NavArea::InitNavConfig();
}
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	NetworkCommunicatorClient client = NetworkCommunicatorClient();
	init_static();
	client.WaitForMessage();
	//WorldViewer viewer = WorldViewer(Game::LocationBase::Get("Kalimdor"), WorldPositionToBlockCoords(client.player_position), WorldPositionToChunkCoords(client.player_position), 3);
	WorldViewer viewer = WorldViewer(client.player_position);
	//WorldViewer viewer = WorldViewer(Game::LocationBase::Get("Kalimdor"), Point2D<int>(34,31/*28, 40*/), Point2D<int>(2,16/*10, 2*/), 3);
	viewer.ShowMap();
	while (1)
	{
		client.WaitForMessage();
		cout << client.player_position.x << " " << client.player_position.y << endl;
		//viewer.Update(Game::LocationBase::Get("Kalimdor"), WorldPositionToBlockCoords(client.player_position), WorldPositionToChunkCoords(client.player_position));
		viewer.Update(client.player_position);
	}
    return 0;
}

