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

	GlobalNavMesh gnm = GlobalNavMesh();
	gnm.Start(0, 0);
	for (int i = 35; i < 40;i++)
		for (int j = 30; j < 38; j++)
		{
			cout << i << ": " << j << endl;
			//gnm.AddTile(Point2DI(i, j));
			
		}

	
	
	
	gnm.AddTile(Point2DI(35, 31));
	gnm.AddTile(Point2DI(35, 32));
	gnm.AddTile(Point2DI(35, 33));
	gnm.AddTile(Point2DI(35, 34));
	gnm.AddTile(Point2DI(35, 35));
	gnm.AddTile(Point2DI(36, 26));

	
	gnm.AddTile(Point2DI(36, 31));
	gnm.AddTile(Point2DI(36, 32));
	gnm.AddTile(Point2DI(36, 33));
	gnm.AddTile(Point2DI(36, 34));
	gnm.AddTile(Point2DI(36, 35));
	
	gnm.AddTile(Point2DI(37, 31));
	gnm.AddTile(Point2DI(37, 32));
	gnm.AddTile(Point2DI(37, 33));
	gnm.AddTile(Point2DI(37, 34));
	gnm.AddTile(Point2DI(37, 35));

	gnm.AddTile(Point2DI(38, 31));
	gnm.AddTile(Point2DI(38, 32));
	gnm.AddTile(Point2DI(38, 33));
	gnm.AddTile(Point2DI(38, 34));
	gnm.AddTile(Point2DI(38, 35));

	gnm.Load();

	//LocalAreaNavMesh nm = LocalAreaNavMesh(LocationBase::Get("Kalimdor"), Point2DI(36, 34), Point2DI(7, 9));
	//nm.Build();


	//Doodad dd = Doodad("D:\\Bot\\Extracted\\world\\kalimdor\\barrens\\passivedoodads\\fx\\barrens_regrow_fx.m2", 0, Position(), 1);
	//WMO dd = WMO("D:\\Bot\\Extracted\\world\\wmo\\kalimdor\\collidabledoodads\\tanaris\\trolltent\\trolltentsand01.wmo", 0, Position());

	//WorldViewer viewer = WorldViewer(LocationBase::Get("Kalimdor"), Point2DI(0, 0), Point2DI(7, 9), 1);


	
	WorldViewer viewer = WorldViewer(LocationBase::Get("Kalimdor"), Point2DI(35, 33), Point2DI(7, 9), 3);
	
	viewer.GetState()->AddNavMesh(gnm.GetPolyMeshes());
	
	//viewer.GetFrame()->AddNavMesh(gnm.GetPolyMeshes());
	viewer.ShowMap();
	while (1)
	{
		Sleep(100);
	}
	return 0;
}

