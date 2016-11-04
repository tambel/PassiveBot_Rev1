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
{/*
	setlocale(LC_ALL, "Russian");
	NetworkCommunicatorClient client = NetworkCommunicatorClient("127.0.0.1", 8001);
	client.Connect();
	init_static();
	RequestPacket<1> p;
	//pr.Pack();
	client.SendPacket<RequestPacket<1>>(p);
	PlayerPositionReply * rep = reinterpret_cast<PlayerPositionReply*>(client.RecievePacket());
	WorldViewer viewer = WorldViewer(rep->position);
	delete rep;
	viewer.ShowMap();

	//WorldViewer viewer = WorldViewer(Game::LocationBase::Get("Kalimdor"), Point2D<int>(28, 40), Point2D<int>(10, 2),3);
	
	while (1)
	{
		client.SendPacket<RequestPacket<1>>(p);
		rep = reinterpret_cast<PlayerPositionReply*>(client.RecievePacket());
		//WorldViewer viewer = WorldViewer(client.player_position);
		viewer.Update(rep->position);
		delete rep;

	}





	*/
    return 0;
}

