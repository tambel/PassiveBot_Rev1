#include "stdafx.h"



WMO::WMO(WMORoot & root, unsigned uuid, Position position):uuid(uuid),position(std::move(position))
{
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

}


WMO::~WMO()
{
	cout << "WMO " << this << " deleted" << endl;
	//for (auto part : parts)
	{
		//delete part;
		//part = 0;
	}
	parts.clear();
}
