// WorldViewer.cpp: определяет точку входа для консольного приложения.
//


#include "stdafx.h"
#include "ViewerClient.h"
#include "Graphics\Graphics.h"

using namespace std;


static void init_static()
{
	Utils::Geometry::Transformer3D::Transformer3D();
	Game::LocationBase::Init();
	NavArea::InitNavConfig();
}
int main()
{
	init_static();

	//GlobalNavMesh gnm = GlobalNavMesh();
	//gnm.Start(0, 0);
	//gnm.AddTile(Point2DI(0, 1));
	//gnm.AddTile(Point2DI(1, 1));
	//gnm.AddTile(Point2DI(1, 3));
	//gnm.AddTile(Point2DI(2, 0));
	//gnm.AddTile(Point2DI(2, 1));
	//gnm.AddTile(Point2DI(2, 2));
	//gnm.AddTile(Point2DI(2, 3));
	//gnm.AddTile(Point2DI(36, 36));
	//gnm.AddTile(Point2DI(37, 36));
	//gnm.AddTile(Point2DI(38, 36));
	//gnm.AddTile(Point2DI(37, 37));
	//gnm.AddTile(Point2DI(38, 37));

	//LocalAreaNavMesh nm = LocalAreaNavMesh(LocationBase::Get("Kalimdor"), Point2DI(36, 34), Point2DI(7, 9));
	//nm.Build();


	//Doodad dd = Doodad("D:\\Bot\\Extracted\\world\\kalimdor\\barrens\\passivedoodads\\fx\\barrens_regrow_fx.m2", 0, Position(), 1);
	//WMO dd = WMO("D:\\Bot\\Extracted\\world\\wmo\\kalimdor\\collidabledoodads\\tanaris\\trolltent\\trolltentsand01.wmo", 0, Position());

	//WorldViewer viewer = WorldViewer(LocationBase::Get("Kalimdor"), Point2DI(0, 0), Point2DI(7, 9), 1);

	WorldViewer viewer = WorldViewer(LocationBase::Get("Kalimdor"), Point2DI(35, 33), Point2DI(7, 9), 3);
	
	//viewer.GetState()->AddNavMesh(gnm.GetPolyMeshes());
	
	//viewer.GetFrame()->AddNavMesh(gnm.GetPolyMeshes());
	viewer.ShowMap();
	while (1)
	{
		Sleep(100);
	}
	return 0;
}

