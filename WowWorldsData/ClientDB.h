#pragma once
#include "DB2Reader.h"
#include "WowTables.h"
#include "WorldMapArea.h"

class ClientDB
{
public:
	ClientDB();
	~ClientDB();
	static vector<WorldMapArea> ReadWorldMapAreas();
};

