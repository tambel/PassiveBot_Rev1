#pragma once
#include "WMOGroup.h"
#include "Tools\BinaryReader.h"
#include "ADTStructs.h"
#include "WMOStructs.h"
#include "WMO.h"
struct WMOInfo
{
	string filename;
	MODF modf;
	WMOInfo(string & filename, MODF modf) : modf(modf) 
	{
		this->filename = filename;
	}
	~WMOInfo() {};
};

class WMORoot
{
	string filename;
	vector<WMOGroup> wmo_groups;
	void LoadGroups(MOHD  & header);
public:
	vector<WMOGroup> & GetWMOGroups() { return wmo_groups; }
	WMORoot(string  & filename);
	~WMORoot();
	vector<WMOGroup> & GetGroups() { return wmo_groups; }
	//friend WMO::WMO(string filename, unsigned uuid, Position & position);
	//friend WMO::~WMO();
};

