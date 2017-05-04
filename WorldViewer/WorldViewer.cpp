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
	
	auto areas = ClientDB::ReadWorldMapAreas();

	auto mulgore = *find_if(areas.begin(), areas.end(), [](auto & a) {return a.name == "Mulgore"; });
	auto durotar = *find_if(areas.begin(), areas.end(), [](auto & a) {return a.name == "Durotar"; });
	auto barrens = *find_if(areas.begin(), areas.end(), [](auto & a) {return a.name == "Barrens"; });
	auto bb = mulgore.GetBoundingBox();


	GlobalNavMesh gnm = GlobalNavMesh();


	auto cbb = Metrics::ConvertFromGameCoords(bb);
	//gnm.Start(0, 0);

	//gnm.AddArea(Point2D<float>(cbb.up.x, cbb.up.z), Point2D<float>(cbb.down.x, cbb.down.z));


	auto c1 = Metrics::ConvertFromGameCoords(Vector3(-2435.32, -530.151, -8.99781));
	auto c2 = Metrics::ConvertFromGameCoords(Vector3(-1344.39, 195.327, 61.0569));

	//gnm.AddArea(Point2D<float>(c1.x,c1.z), Point2D<float>(c2.x,c2.z));
	//gnm.AddArea(Point2D<float>(0, 0), Point2D<float>(3000, 3000));

	//gnm.Save();

	gnm.Load();

	WorldViewer viewer = WorldViewer(LocationBase::Get("Kalimdor"), Point2DI(35, 32), Point2DI(7, 9), 3);
	//viewer.GetArea().ToMesh();
	
	

	
	//gnm.FindPath(Vector3(-2435.32, -530.151,-8.99781), Vector3(-1344.39,195.327,61.0569));
	//auto v1 = gnm.GetLastPath();
	//viewer.GetState()->AddLineStrip(v1);


	for (auto & a : areas)
	{
		//viewer.GetState()->AddBoundingBox2D(Utils::Metrics::ConvertFromGameCoords(a.GetBoundingBox()));
	}

	viewer.GetState()->AddBoundingBox2D(Utils::Metrics::ConvertFromGameCoords(bb));
	//viewer.GetState()->AddBoundingBox2D(Utils::Metrics::ConvertFromGameCoords(durotar.GetBoundingBox()));
	//viewer.GetState()->AddBoundingBox2D(Utils::Metrics::ConvertFromGameCoords(barrens.GetBoundingBox()));


	viewer.GetState()->AddNavMesh(gnm.GetNavMesh(),gnm.GetNavMeshOffset());
	
	
	//viewer.GetFrame()->AddNavMesh(gnm.GetPolyMeshes());
	viewer.ShowMap();
	while (1)
	{
		Sleep(100);
	}
	return 0;
}

