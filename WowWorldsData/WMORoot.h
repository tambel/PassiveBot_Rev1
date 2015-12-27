#pragma once
#include "WMOStructs.h"
#include "WMOGroup.h"
#include <vector>
struct WMOInfo
{
	string filename;
	MODF modf;
	WMOInfo(string filename, MODF modf) : filename(filename), modf(modf) {}
	
};

class WMORoot
{
	string filename;
	vector<WMOGroup> wmo_groups;
	void LoadGroups(MOHD  & header);
public:
	vector<WMOGroup> & GetWMOGroups() { return wmo_groups; }
	WMORoot(string filename);
	~WMORoot();
};

