#pragma once
#include "WMOPart.h"
class WMO
{
	vector<WMOPart> parts;
	unsigned long uuid;
	Position position;
public:
	WMO(WMORoot & root, unsigned uuid, Position position);
	WMO(WMO && right);
	~WMO();
	unsigned long GetUUID() { return uuid; }
	
	Position & GetPosition() { return position; }
	vector<WMOPart> & GetParts() { return parts; }

};

