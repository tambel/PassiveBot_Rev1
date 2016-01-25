#pragma once

class WMOGroup
{

	
public:
	string filename;
	Utils::WowTypes::Vector3 * vertices;
	unsigned long vertex_count;
	unsigned short * indices;
	unsigned long index_count;
	WMOGroup(string filename);
	WMOGroup(WMOGroup && right);
	
	~WMOGroup();
};

