#pragma once
#include "MapObject.h"
#include "WMOPart.h"
#include "WMORoot.h"
class WMO :public MapObject,public Model
{
	unsigned long uuid;
	inline void _move(WMO & other);
public:
	//WMO(WMORoot & root, unsigned uuid, Position position);
	WMO(string filename, unsigned uuid, Position & position);
	WMO(WMO && right);
	~WMO();
	WMO & operator=(WMO && right);
	unsigned long GetUUID() { return uuid; }

};

