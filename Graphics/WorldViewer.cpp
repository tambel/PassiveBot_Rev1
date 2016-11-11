#include "stdafx.h"
#include "WorldViewer.h"

void thread_func(MapFrame * frame)
{
	frame->go();
}
 
WorldViewer::WorldViewer(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius)
{
	frame = MapFrame();
	area = MutableArea(location,block_coordinates,coordinates,radius);
	frame.SetArea(&area);

}

WorldViewer::WorldViewer(Vector3 & position)
{
	//WorldViewer(Game::LocationBase::Get("Kalimdor"), WorldPositionToBlockCoords(position), WorldPositionToChunkCoords(position), 3);
	frame = MapFrame();
	area = MutableArea(Game::LocationBase::Get("Kalimdor"), WorldPositionToBlockCoords(position), WorldPositionToChunkCoords(position), 30);
	frame.SetArea(&area);
	
	frame.SetPlayerPosition(position);

	

}


WorldViewer::~WorldViewer()
{
}

void WorldViewer::ShowMap()
{
	frame_thread = thread(thread_func,&frame);
	frame_thread.detach();
}

void WorldViewer::Update(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates)
{
	
	area.CheckAndUpdate(location, block_coordinates, coordinates);

}
void WorldViewer::Update(Vector3 & position)
{
	Update(Game::LocationBase::Get("Kalimdor"), WorldPositionToBlockCoords(position), WorldPositionToChunkCoords(position));
	frame.SetPlayerPosition(position);
}

WorldViewer & WorldViewer::operator=(WorldViewer && other)
{
	return *this;
}
WorldViewer::WorldViewer(WorldViewer && other)
{

}