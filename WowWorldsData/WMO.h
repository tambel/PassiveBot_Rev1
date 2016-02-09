#pragma once
#include "MapObject.h"
#include "WMOPart.h"
class WMO :public MapObject,public Model
{
	vector<WMOPart> parts;
	unsigned long uuid;
	//Position position;
	bool skip;
	inline void _move(WMO & other);
	Utils::Graphics::BoundingBox bounding_box;

	
public:

	//WMO(WMORoot & root, unsigned uuid, Position position);
	WMO(string filename, unsigned uuid, Position position);
	WMO(WMO && right);
	~WMO();
	WMO & operator=(WMO && right);
	unsigned long GetUUID() { return uuid; }
	
	//Position & GetPosition() { return position; }
	vector<WMOPart> & GetParts() { return parts; }
	void Skip() { skip = true; }
	inline bool  IsSkipped() { return skip; }
	inline Utils::Graphics::BoundingBox & GetBoundingBox() { return bounding_box; }

};

