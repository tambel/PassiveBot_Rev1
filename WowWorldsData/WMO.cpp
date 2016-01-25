#include "stdafx.h"



void WMO::_move(WMO & other)
{
	MapObject::swap(other);
	position = other.position;
	uuid = other.uuid;
	other.uuid = 0;
	skip = other.skip;
}

WMO::WMO(string filename, unsigned uuid, Position position):
	MapObject(filename),
	uuid(uuid),position(position)
{
	skip = false;
	WMORoot root = WMORoot(this->filename);
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

	//_move(right);
	MapObject::operator=(move(right));
	position = right.position;
	uuid = right.uuid;
	right.uuid = 0;
	skip = right.skip;

}


WMO::~WMO()
{
	parts.clear();
}

WMO & WMO::operator=(WMO && right)
{
	//_move(right);
	MapObject::operator=(move(right));
	position = right.position;
	uuid = right.uuid;
	right.uuid = 0;
	skip = right.skip;
	return *this;
}
