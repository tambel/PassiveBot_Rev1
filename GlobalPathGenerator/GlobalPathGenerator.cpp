// GlobalPathGenerator.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "PathGenerator.h"


void init_static()
{
	Utils::Geometry::Transformer3D::Transformer3D();
	Game::LocationBase::Init();
	NavArea::InitNavConfig();
}

int main()
{
	
	init_static();
	PathGenerator gen;
	if (!gen.IsOnSavedState())
	{
		gen.StartNew(Game::LocationBase::Get("Kalimdor"), Point2D<int>(0, 0), Point2D<int>(0, 0));
		return 0;
	}
	int c = 0;
	cout << "There is saved state from previous generation. Do you want to resume it?" << endl << "y/n ";

	while (c != 'n' && c != 'y')
		c = getchar();
	c == 'y' ? gen.Continue() : gen.StartNew(Game::LocationBase::Get("Kalimdor"), WorldPositionToBlockCoords(Vector3(280.83,-4749.3f,9.6f)), WorldPositionToChunkCoords(Vector3(280.83, -4749.3f, 9.6f)), Point2D<int>(31, 40), Point2D<int>(15, 15));

	//c == 'y' ? gen.Continue() : gen.StartNew(Game::LocationBase::Get("Kalimdor"), Point2D<int>(31, 40), Point2D<int>(11, 14), Point2D<int>(31, 40), Point2D<int>(15, 15));
    return 0;
}

