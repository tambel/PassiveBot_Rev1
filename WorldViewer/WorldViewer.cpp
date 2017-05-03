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

	
	
	//gnm.Save();
	//

	//LocalAreaNavMesh nm = LocalAreaNavMesh(LocationBase::Get("Kalimdor"), Point2DI(36, 34), Point2DI(7, 9));
	//nm.Build();


	//Doodad dd = Doodad("D:\\Bot\\Extracted\\world\\kalimdor\\barrens\\passivedoodads\\fx\\barrens_regrow_fx.m2", 0, Position(), 1);
	//WMO dd = WMO("D:\\Bot\\Extracted\\world\\wmo\\kalimdor\\collidabledoodads\\tanaris\\trolltent\\trolltentsand01.wmo", 0, Position());

	//WorldViewer viewer = WorldViewer(LocationBase::Get("Kalimdor"), Point2DI(0, 0), Point2DI(7, 9), 1);


	
	WorldViewer viewer = WorldViewer(LocationBase::Get("Kalimdor"), Point2DI(35, 33), Point2DI(7, 9), 3);
	//viewer.GetArea().ToMesh();
	
	auto areas = ClientDB::ReadWorldMapAreas();

	auto mulgore = *find_if(areas.begin(), areas.end(), [](auto & a) {return a.name == "Mulgore"; });
	auto durotar  = *find_if(areas.begin(), areas.end(), [](auto & a) {return a.name == "Durotar"; });
	auto barrens = *find_if(areas.begin(), areas.end(), [](auto & a) {return a.name == "Barrens"; });
	auto bb = mulgore.GetBoundingBox();

	auto is_intersect = [](BoundingBox & b1, BoundingBox & b2)
	{
		if (b1.IsInside2D(b2.up) || b1.IsInside2D(Vector3(b2.up.x, b2.down.y, 0)) || (b1.IsInside2D(b2.down)) || b1.IsInside2D(Vector3(b2.down.x, b2.up.y, 0)))
		{
			return true;
		}
		return false;
	};
	vector<WorldMapArea> neigh;
	for (auto & a : areas)
	{
		if (a.name != mulgore.name)
		{
			if (is_intersect(mulgore.GetBoundingBox(), a.GetBoundingBox()))
			{
				neigh.push_back(a);
			}
		}
	}



	
	
	gnm.FindPath(Vector3(-2435.32, -530.151,-8.99781), Vector3(-1344.39,195.327,61.0569));
	auto v1 = gnm.GetLastPath();
	viewer.GetState()->AddLineStrip(v1);

	float w = abs(mulgore.right-mulgore.left)/256;
	float h = abs(mulgore.bottom - mulgore.top)/256;
	cout << w << " " << h << endl;
	double sq = w*h;
	cout << sq << endl;

	for (auto & a : areas)
	{
		//viewer.GetState()->AddBoundingBox2D(Utils::Metrics::ConvertFromGameCoords(a.GetBoundingBox()));
	}

	viewer.GetState()->AddBoundingBox2D(Utils::Metrics::ConvertFromGameCoords(bb));
	//viewer.GetState()->AddBoundingBox2D(Utils::Metrics::ConvertFromGameCoords(durotar.GetBoundingBox()));
	//viewer.GetState()->AddBoundingBox2D(Utils::Metrics::ConvertFromGameCoords(barrens.GetBoundingBox()));


	//viewer.GetState()->AddNavMesh(gnm.GetNavMesh());
	
	
	//viewer.GetFrame()->AddNavMesh(gnm.GetPolyMeshes());
	viewer.ShowMap();
	while (1)
	{
		Sleep(100);
	}
	return 0;
}

