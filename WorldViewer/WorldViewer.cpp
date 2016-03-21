// WorldViewer.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include "Graphics\WorldViewer.h"
#include <iostream>
#include <conio.h>
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
	//NetworkCommunicatorClient client = NetworkCommunicatorClient();
	init_static();
	//client.WaitForMessage();
	//WorldViewer viewer = WorldViewer(client.player_position);
	WorldViewer viewer = WorldViewer(Vector3(280.83, -4749.3f, 9.6f));
	viewer.ShowMap();
	//getchar();
	while (1)
	{
		//client.WaitForMessage();
		//cout << client.player_position.x << " " << client.player_position.y << " "<< client.player_position.z<<endl;
		viewer.Update(Vector3(280.83, -4749.3f, 9.6f));
		//viewer.Update(client.player_position);

	}

    return 0;
}

