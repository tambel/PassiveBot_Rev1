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
	CascReader::OpenStorage();
}
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	//NetworkCommunicatorClient client = NetworkCommunicatorClient();
	init_static();
	//client.WaitForMessage();
	//WorldViewer viewer = WorldViewer(client.player_position);
	

	WorldViewer viewer = WorldViewer(Game::LocationBase::Get("Kalimdor"), Point2D<int>(28, 40), Point2D<int>(10, 2),3);
	
	




	viewer.ShowMap();
	getch();
	Point2D<int> block;
	Point2D<int>chunk;
	Point2D<int>ob, oc;
	string s;
	for (int i = 0; i < 1024; i++)
	{
		block.X = i / 16;
		chunk.X = i - block.X * 16;
		for (int j = 0; j < 1024; j++)
		{
			block.Y = j / 16;
			chunk.Y = j - block.Y * 16;
			try
			{
				//viewer.Update(Game::LocationBase::Get("Kalimdor"), block, chunk);
				viewer.GetFrame().GetArea()->UpdateArea(Game::LocationBase::Get("Kalimdor"), block, chunk);
			}
			catch (EmptyAreaException & e)
			{
				cout << e.what() << endl;;
			}
			cout << block.X << " " << block.Y << endl << chunk.X << " " << chunk.Y << endl << "==========" << endl;
			//cin >>s;
			//getch();

		}
	}



	while (1)
	{
		//client.WaitForMessage();
		
		//viewer.Update(client.player_position);

	}

    return 0;
}

