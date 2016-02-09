#pragma once

#include "WMO.h"

class WMO;
class WMOGroup;
class WMOPart:public Model
{
	string filename;
public:
	WMOPart(WMOGroup & group, WMO & wmo);
	WMOPart(WMOPart && right);
	~WMOPart();
};

