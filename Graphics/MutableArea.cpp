#include "stdafx.h"
#include "MutableArea.h"

MutableArea::MutableArea() :area_type()
{

	to_update = false;
}

MutableArea::MutableArea(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius) :
	area_type(location, block_coordinates, coordinates, radius)
{
	to_update = false;
}

MutableArea & MutableArea::operator=(MutableArea && other)
{
	area_type::operator=(move(other));
	return *this;
}

void MutableArea::Update(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates)
{
	lock_guard<mutex> guard(data_mutex);
	try
	{
		area_type::Update(location, block_coordinates, coordinates);
	}
	catch (EmptyAreaException & e)
	{
		to_update = false;
		throw e;
	}
	to_update = true;
}

