// WorldViewer.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Graphics\WorldViewer.h"


void init_static()
{
	Utils::Geometry::Transformer3D::Transformer3D();
	Game::LocationBase::Init();
	NavArea::InitNavConfig();
}
int main()
{
	init_static();
	WorldViewer viewer = WorldViewer(Game::LocationBase::Get("Kalimdor"), Point2D<int>(33, 41), Point2D<int>(10, 3), 3);
	viewer.ShowMap();
	while (1)
	{

	}
    return 0;
}

