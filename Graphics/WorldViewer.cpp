#include "stdafx.h" 
#include "WorldViewer.h"

#include "MapState.h"

void thread_func(MapFrame * frame)
{
	frame->go();
}

WorldViewer::WorldViewer(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius)
{
	state = new MapState("STATE");
	area = Area(radius, AreaFormat::fBlock);
	area.UpdateArea(location, block_coordinates, coordinates);

	state->SetArea(&area);

	frame = new MapFrame(state);
	state->_notifyGraphicsSystem(frame);

}

WorldViewer::WorldViewer(Vector3 & position)
{
	//WorldViewer(Game::LocationBase::Get("Kalimdor"), WorldPositionToBlockCoords(position), WorldPositionToChunkCoords(position), 3);
	//frame = new MapFrame();
	//area = MutableArea(Game::LocationBase::Get("Kalimdor"), WorldPositionToBlockCoords(position), WorldPositionToChunkCoords(position), 0);
	//frame.SetArea(&area);

	//frame.SetPlayerPosition(position);



}


WorldViewer::~WorldViewer()
{
}

void WorldViewer::ShowMap()
{
	frame_thread = thread(thread_func, frame);
	frame_thread.detach();
}

void WorldViewer::Update(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates, bool force)
{
	//if (force)
	//	area.Update(location, block_coordinates, coordinates);
	//else
	//	area.CheckAndUpdate(location, block_coordinates, coordinates);

}
void WorldViewer::Update(Vector3 & position)
{
	Update(Game::LocationBase::Get("Kalimdor"), WorldPositionToBlockCoords(position), WorldPositionToChunkCoords(position));
	//frame.SetPlayerPosition(position);
}

WorldViewer & WorldViewer::operator=(WorldViewer && other)
{
	return *this;
}
WorldViewer::WorldViewer(WorldViewer && other)
{

}