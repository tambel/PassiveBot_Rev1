#include "stdafx.h"
#include "MapObject.h"

unsigned MapObject::lifetime = 5;
inline void MapObject::swap(MapObject & other)
{
	filename = move(other.filename);
	unused_iter_count = other.unused_iter_count;
	refreshed = other.refreshed;
}
MapObject::MapObject()
{
	//unused_iter_count = 0;
	//refreshed = true;
}
MapObject::MapObject(string filename)
{
	this->filename = move(filename);
	/*unused_iter_count = 0;
	refreshed = true;*/
}


MapObject::~MapObject()
{
	
}

MapObject::MapObject(MapObject && other)
{
	swap(other);
}

MapObject & MapObject::operator=(MapObject && right)
{
	swap(right);
	return *this;
}

void MapObject::HitUnuseed()
{
	unused_iter_count++;
	refreshed = false;
}

void MapObject::Refresh()
{
	unused_iter_count = 0;
	refreshed = true;
}

