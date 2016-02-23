#include "stdafx.h"
#include "WorldViewer.h"

void thread_func(MapFrame & frame)
{
	frame.go();
}
 
WorldViewer::WorldViewer(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius)
{
	frame = MapFrame();
	area = MutableArea(location,block_coordinates,coordinates,radius);
	frame.SetArea(&area);
}


WorldViewer::~WorldViewer()
{
}

void WorldViewer::ShowMap()
{
	frame_thread = thread(thread_func,frame);
	frame_thread.detach();
}

void WorldViewer::Update(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates)
{
	
	area.CheckAndUpdate(location, block_coordinates, coordinates);

}
WorldViewer & WorldViewer::operator=(WorldViewer && other)
{
	return *this;
}
WorldViewer::WorldViewer(WorldViewer && other)
{

}