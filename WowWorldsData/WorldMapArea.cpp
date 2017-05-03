#include "stdafx.h"
#include "WorldMapArea.h"


WorldMapArea::WorldMapArea(WorldMapAreaRec & rec, string & name): name(name)
{
	id = rec.id;
	bottom = rec.bottom;
	right = rec.right;
	left = rec.left;
	top = rec.top;
	bb.up = Vector3(top, left, 0);
	bb.down = Vector3( bottom,right, 0);
}


WorldMapArea::~WorldMapArea()
{
}
