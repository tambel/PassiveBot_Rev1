#include "stdafx.h"
#include "MapObject.h"

unsigned MapObject::lifetime = 2;
inline void MapObject::swap(MapObject & other)
{
	name = move(other.name);
	unused_iter_count = other.unused_iter_count;
	refreshed = other.refreshed;
}
MapObject::MapObject()
{
	//unused_iter_count = 0;
	//refreshed = true;
}
MapObject::MapObject(string name)
{
	this->name = move(name);
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

