// WorldViewer.cpp: определяет точку входа для консольного приложения.
//


#include "stdafx.h"
#include "ViewerClient.h"
#include "Graphics\Graphics.h"

using namespace std;

class CLI
{
};

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

	for (auto a : areas)
	{
		if (a.id == 4796)
		{
			cout << a.name << endl;
		}
	}

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

	
	//viewer.GetArea().ToMesh();
	
	

	
	gnm.FindPath(Vector3(-2435.32, -530.151,-8.99781), Vector3(-1344.39,195.327,61.0569));
	


	WorldViewer viewer = WorldViewer(LocationBase::Get("Kalimdor"), Point2DI(35, 32), Point2DI(7, 9), 3);

	auto v1 = gnm.GetLastPath();
	//viewer.GetState()->AddLineStrip(v1);

	//viewer.GetState()->AddBoundingBox2D(Utils::Metrics::ConvertFromGameCoords(bb));


	//viewer.GetState()->AddNavMesh(gnm.GetNavMesh(),gnm.GetNavMeshOffset());
	
	ViewerClient client= ViewerClient();
	client.Connect();
	//viewer.GetFrame()->AddNavMesh(gnm.GetPolyMeshes());
	viewer.ShowMap();
	string command;
	Vector3 start, end;

	while (1)
	{
		cin >> command;
		if (command=="start")
			start= client.GetPlayerPosition().coords;

		if (command == "end")
		{
			start = client.GetPlayerPosition().coords;
		}
		if (command == "commit")
		{
			cout << "commit" << endl;
			//viewer.GetState()->AddMap();
			viewer.GetState()->AddBoundingBox2D(Utils::Metrics::ConvertFromGameCoords(bb));
			//viewer.GetState()->AddNavMesh2(gnm.GetNavMesh(), gnm.GetNavMeshOffset());
			
			//gnm.FindPath(start, end);

		}
		//cout << "send Request" << endl;

		//auto pos = client.GetPlayerPosition();
		//cout << pos.coords.x << " : " << pos.coords.y << " : " << pos.coords.z << endl;
		//Sleep(100);
	}
	return 0;
}

