#include "stdafx.h"
#include "MutableArea.h"

MutableArea::MutableArea() :NavArea()
{
	to_update = false;
}

MutableArea::MutableArea(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius) :
	NavArea(location, block_coordinates, coordinates, radius) 
{
	to_update = false;
}

MutableArea & MutableArea::operator=(MutableArea && other)
{
	NavArea::operator=(move(other));
	return *this;
}

void MutableArea::Update(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates)
{
	lock_guard<mutex> guard(data_mutex);
	try
	{
		NavArea::Update(location, block_coordinates, coordinates);
	}
	catch (EmptyAreaException & e)
	{
		to_update = false;
		throw e;
	}
	to_update = true;
}

