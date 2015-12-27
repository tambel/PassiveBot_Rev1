#pragma once

#include "WMO.h"

class WMO;
class WMOPart:public Model<unsigned short>
{
	string filename;
public:
	WMOPart(WMOGroup & group, WMO & wmo);
	WMOPart(WMOPart && right);
	~WMOPart();
};

