#pragma once
#include "MapFrame.h"
#include "MutableArea.h"
using namespace std;



class WorldViewer
{

	MapFrame frame;
	thread frame_thread;
	mutex area_mutex;
	MutableArea area;
public:
	WorldViewer(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius);
	WorldViewer(Vector3 & position);
	WorldViewer(WorldViewer && other);
	~WorldViewer();
	void ShowMap();
	void Update(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates);
	void Update(Vector3 & position);
	WorldViewer & operator=(WorldViewer && other);
	inline MapFrame & GetFrame() { return frame; }

};

