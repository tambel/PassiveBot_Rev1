// WorldViewer.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include "Graphics\WorldViewer.h"
#include <iostream>
#include <conio.h>
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
	WorldViewer viewer = WorldViewer(client.GetPlayerPosition().coords);
	viewer.ShowMap();
	while (1)
	{
		viewer.Update(client.GetPlayerPosition().coords);
		Sleep(100);
	}
    return 0;
}

