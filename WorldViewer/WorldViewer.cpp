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
	int tc = 0;
	GlobalNavMesh gnm = GlobalNavMesh();
	//gnm.Start(0, 0);
	int si = 224;
	int sj = 210;
	for (int i = si; i < si+30;i++)
		for (int j = sj; j < sj+30; j++)
		{
			cout << i << ": " << j << endl;
			tc++;
			//gnm.AddTile(Point2DI(i, j));
			
		}
	//gnm.Save();
	cout << tc<<endl;
	gnm.Load();
	

	//gnm.FindPath(Vector3(-2334, -369.355, -8.50156), Vector3(-1337.96, 174.69, 58.2956));

	//gnm.FindPath(Vector3(-2334, -369.355, -8.50156), Vector3(-2223.09,-386.793, -9.42134));
	
	//gnm.FindPath(Vector3(-2334, -369.355, -8.50156), Vector3(-2339, -375.355, -8.50156));
	int i = 35;
	int j = 32;
	
	/*
	gnm.AddTile(Point2DI(i, j));
	
	gnm.AddTile(Point2DI(35, 33));
	gnm.AddTile(Point2DI(35, 34));
	gnm.AddTile(Point2DI(34, 35));
	gnm.AddTile(Point2DI(35, 35));
	gnm.AddTile(Point2DI(36, 35));

	gnm.AddTile(Point2DI(36, 34));
	gnm.AddTile(Point2DI(36, 33));
	gnm.AddTile(Point2DI(36, 32));
	gnm.AddTile(Point2DI(37, 35));

	gnm.AddTile(Point2DI(37, 34));
	gnm.AddTile(Point2DI(37, 33));
	gnm.AddTile(Point2DI(37, 32));
	*/

	
	
	//gnm.Save();
	//

	//LocalAreaNavMesh nm = LocalAreaNavMesh(LocationBase::Get("Kalimdor"), Point2DI(36, 34), Point2DI(7, 9));
	//nm.Build();


	//Doodad dd = Doodad("D:\\Bot\\Extracted\\world\\kalimdor\\barrens\\passivedoodads\\fx\\barrens_regrow_fx.m2", 0, Position(), 1);
	//WMO dd = WMO("D:\\Bot\\Extracted\\world\\wmo\\kalimdor\\collidabledoodads\\tanaris\\trolltent\\trolltentsand01.wmo", 0, Position());

	//WorldViewer viewer = WorldViewer(LocationBase::Get("Kalimdor"), Point2DI(0, 0), Point2DI(7, 9), 1);


	
	WorldViewer viewer = WorldViewer(LocationBase::Get("Kalimdor"), Point2DI(35, 33), Point2DI(7, 9), 3);
	//viewer.GetArea().ToMesh();
	

	
	gnm.FindPath(Vector3(-2435.32, -530.151,-8.99781), Vector3(-1344.39,195.327,61.0569));


	auto v1 = gnm.GetLastPath();
	viewer.GetState()->AddLineStrip(v1);


	//viewer.GetState()->AddNavMesh(gnm.GetNavMesh());
	
	
	//viewer.GetFrame()->AddNavMesh(gnm.GetPolyMeshes());
	viewer.ShowMap();
	while (1)
	{
		Sleep(100);
	}
	return 0;
}

