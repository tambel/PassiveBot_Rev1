#pragma once
#include "WowTables.h"
#include "Utils.h"
using namespace Utils::Geometry;
class WorldMapArea
{
public:
	unsigned short id;
	string name;
	float left;
	float right;
	float top;
	float bottom;
	BoundingBox bb;
	WorldMapArea(WorldMapAreaRec & rec, string & name);
	~WorldMapArea();
	inline BoundingBox GetBoundingBox() { return  bb; }
};

