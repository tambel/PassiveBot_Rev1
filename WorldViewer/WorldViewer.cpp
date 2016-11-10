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
int main(int argc, char* argv[])
{
	init_static();

	ClientDB::GetWorldMapAreas();


	ViewerClient client = ViewerClient();
	
	client.Connect();
	
	Generator gen = Generator();
	Point2DI b = WorldPositionToBlockCoords(client.GetPlayerPosition().coords);
	Point2DI c = WorldPositionToChunkCoords(client.GetPlayerPosition().coords);

	for (int x = c.X - 2; x <= c.X + 2; x++)
	{
		for (int y = c.Y - 2; y <= c.Y + 2; y++)
		{
			gen.LinkChunkWithNeighbours(b, Point2DI(x,y));
		}
	}
	/*Point2DI cc = c;
	gen.LinkChunkWithNeighbours(b,c );
	cc = Point2DI(c.X + 1, c.Y);
	gen.LinkChunkWithNeighbours(b, cc);
	cc = Point2DI(c.X, c.Y+1);
	gen.LinkChunkWithNeighbours(b, cc);
	cc = Point2DI(c.X + 1, c.Y+1);
	gen.LinkChunkWithNeighbours(b, cc);*/


	WorldViewer viewer = WorldViewer(client.GetPlayerPosition().coords);
	Vector3 p[1000];
	for (auto & link : gen.GetLinks())
	{
		/*p[0] = link->point1->point;
		memcpy(p +1, &link.get()->medium_points[0], link->medium_points.size() * 12);
		p[link->medium_points.size()+1] = link->point2->point;
		viewer.GetFrame().AddPath(p, link->medium_points.size() + 2);*/
		viewer.GetFrame().AddPath(link.get());
	}
	viewer.ShowMap();
	while (1)
	{
		//viewer.Update(client.GetPlayerPosition().coords);
		Sleep(100);
	}
    return 0;
}

int main2()
{
	init_static();
	WorldViewer viewer = WorldViewer(LocationBase::Get("Kalimdor"), Point2DI(36, 32), Point2DI(7, 5),1);
	viewer.ShowMap();
	while (1)
	{
		//viewer.Update(Point2DI(36, 32), Point2DI(7, 4));
		Sleep(100);
	}
	return 0;
}

