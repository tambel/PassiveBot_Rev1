#pragma once
#include "WMO.h"
//#include "Tools\BinaryReader.h"
#include "WMOStructs.h"
#include "ChunkedData.h"

//class WMO;
class WMOGroup
{

	unsigned long vertex_count;
	float  * vertices;
	unsigned short * indices;
	unsigned long index_count;
public:
	
	string filename;
	//Utils::WowTypes::Vector3 * vertices;
	
	WMOGroup(string filename);
	WMOGroup(WMOGroup && right);
	unsigned long GetIndexCount() { return index_count; }
	float  * GetVertices() { return vertices; }
	unsigned long  GetVertexCount() { return vertex_count; }
	unsigned short * GetIndices() { return indices; }
	//friend WMO::WMO(string filename, unsigned uuid, Position & position);
	
	~WMOGroup();
};

