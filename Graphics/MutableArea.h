#pragma once
#include <mutex>
#include "WowWorldsData\NavArea.h"


class MutableArea :public NavArea
{
public:
	//mutex data_mutex;
	mutex data_mutex;
	bool to_update=false;
	MutableArea() ;
	MutableArea(int radius);
	//MutableArea(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius);
	~MutableArea() {}
	MutableArea & operator=(MutableArea && other);
	virtual void Update(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates);

};

