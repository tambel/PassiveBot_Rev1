#pragma once
#include "WMOPart.h"
class WMO
{
	vector<WMOPart> parts;
	unsigned long uuid;
	Position position;
	bool skip;
public:
	//WMO(WMORoot & root, unsigned uuid, Position position);
	WMO(string filename, unsigned uuid, Position position);
	WMO(WMO && right);
	~WMO();
	unsigned long GetUUID() { return uuid; }
	
	Position & GetPosition() { return position; }
	vector<WMOPart> & GetParts() { return parts; }
	void Skip() { skip = true; }
	inline bool  IsSkipped() { return skip; }

};

