// WorldViewer.cpp: определяет точку входа для консольного приложения.
//


#include "stdafx.h"
#include "ViewerClient.h"


using namespace std;


void init_static()
{
	Utils::Geometry::Transformer3D::Transformer3D();
	Game::LocationBase::Init();
	NavArea::InitNavConfig();
}
int main2(int argc, char* argv[])
{
	init_static();
	Generator gen = Generator();


	ViewerClient client = ViewerClient();

	client.Connect();

	
	Point2DI b = WorldPositionToBlockCoords(client.GetPlayerPosition().coords);
	Point2DI c = WorldPositionToChunkCoords(client.GetPlayerPosition().coords);
	//gen.LinkChunkWithNeighbours(b, c);
	for (int x = c.X - 2; x <= c.X + 2; x++)
	{
		for (int y = c.Y - 2; y <= c.Y + 2; y++)
		{
			//gen.LinkChunkWithNeighbours(b, Point2DI(x,y));
		}
	}

	auto add_map = [](string name, MapFrame * frame)
	{
		auto a = ClientDB::GetWorldMapArea(name);
		Vector3 * points=new Vector3[5];
		points[0].x = a.m_locTop;
		points[0].z = a.m_locRight;
		points[0].y = -250.0;

		points[1].x = a.m_locBottom;
		points[1].z = a.m_locRight;
		points[1].y = -250.0;

		points[2].x = a.m_locBottom;
		points[2].z = a.m_locLeft;
		points[2].y = -250.0;

		points[3].x = a.m_locTop;
		points[3].z = a.m_locLeft;
		points[3].y = -250.0;

		points[4].x = a.m_locTop;
		points[4].z = a.m_locRight;
		points[4].y = -250.0;
		for (int i = 0; i < 5; i++)
		{
			Vector3 coords = points[i];
			coords = Vector3(Metrics::MapMidPoint - coords.z, coords.y, -(Metrics::MapMidPoint - coords.x));
			points[i] = coords;
		}
		frame->AddStrip(points, 5);
	};

	
	WorldViewer viewer = WorldViewer(client.GetPlayerPosition().coords);
	add_map("Mulgore", viewer.GetFrame());
	add_map("SouthernBarrens", viewer.GetFrame());
	add_map("Barrens", viewer.GetFrame());
	add_map("Durotar", viewer.GetFrame());
	add_map("Ashenvale", viewer.GetFrame());
	//viewer.GetFrame()->AddStrip(points, 5);

	for (auto & link : gen.GetLinks())
	{
		/*p[0] = link->point1->point;
		memcpy(p +1, &link.get()->medium_points[0], link->medium_points.size() * 12);
		p[link->medium_points.size()+1] = link->point2->point;
		viewer.GetFrame().AddPath(p, link->medium_points.size() + 2);*/
		viewer.GetFrame()->AddPath(link.get());
	}
	viewer.ShowMap();
	while (1)
	{
		//viewer.Update(client.GetPlayerPosition().coords);
		Sleep(100);
	}
	return 0;
}

int main()
{
	init_static();
	//Generator gen = Generator();
	GlobalNavMesh nm = GlobalNavMesh();
	nm.Start(0, 0);
	nm.AddTile(Point2DI(0, 1));
	nm.AddTile(Point2DI(0, 2));
	nm.AddTile(Point2DI(1, 0));
	nm.AddTile(Point2DI(1, 1));
	///nm.AddTile(Point2DI(38, 36));
	//nm.AddTile(Point2DI(37, 37));
	//nm.AddTile(Point2DI(38, 37));
	WorldViewer viewer = WorldViewer(LocationBase::Get("Kalimdor"), Point2DI(36, 34), Point2DI(7, 9), 2);
	viewer.GetFrame()->AddNavMesh(nm.GetPolyMeshes());
	viewer.ShowMap();
	while (1)
	{
		dynamic_cast<Area*>(&viewer.GetArea())->Rotate();
		Sleep(100);
	}
	return 0;
}

