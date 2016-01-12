#include "stdafx.h"



WMO::WMO(string filename, unsigned uuid, Position position):uuid(uuid),position(std::move(position))
{
	skip = false;
	WMORoot root = WMORoot(filename);
	this->position.coords = Vector3(position.coords.x, -position.coords.z, position.coords.y);
	parts = vector<WMOPart>();
	for (auto &group : root.GetWMOGroups())
	{
		parts.push_back(WMOPart(group,*this));
	}
	for (auto &part : parts)
	{
		part.Rotate();
	}

}

WMO::WMO(WMO && right):parts(std::move(right.parts))
{
	//parts = move(right.parts);
	position = right.position;
	uuid = right.uuid;
	skip = right.skip;
}


WMO::~WMO()
{
	parts.clear();
}
