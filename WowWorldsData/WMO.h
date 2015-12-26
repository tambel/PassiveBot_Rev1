#pragma once
#include "WMOPart.h"
class WMO
{
	vector<WMOPart> parts;
	unsigned long uuid;
public:
	unsigned long GetUUID() { return uuid; }
	WMO(WMORoot & root, unsigned uuid, Position position);

	~WMO();
};

