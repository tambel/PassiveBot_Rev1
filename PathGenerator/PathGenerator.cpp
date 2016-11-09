// PathGenerator.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Generator.h"


void init_static()
{
	Utils::Geometry::Transformer3D::Transformer3D();
	Game::LocationBase::Init();
	NavArea::InitNavConfig();
}

int main()
{
	init_static();
	Generator gen;
	gen.Generate(Point2DI(36, 32), Point2DI(7, 4));
    return 0;
}

