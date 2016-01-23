#pragma once
#include <mutex>
#include "WowWorldsData\NavArea.h"
class MutableArea :public NavArea
{
public:
	mutex data_mutex;
	bool to_update;
	MutableArea() ;
	MutableArea(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius);
	~MutableArea() {}
	MutableArea & operator=(MutableArea && other);
	virtual void CheckAndMove(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates);

};

