#pragma once

class WMOGroup
{

	unsigned long vertex_count;
public:
	
	string filename;
	//Utils::WowTypes::Vector3 * vertices;
	float  * vertices;
	unsigned short * indices;
	unsigned long index_count;
	WMOGroup(string filename);
	WMOGroup(WMOGroup && right);
	friend WMO::WMO(string filename, unsigned uuid, Position & position);
	
	~WMOGroup();
};

