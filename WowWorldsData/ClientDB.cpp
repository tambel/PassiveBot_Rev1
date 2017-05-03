#include "stdafx.h"
#include "ClientDB.h"


ClientDB::ClientDB()
{
}


ClientDB::~ClientDB()
{
}

vector<WorldMapArea> ClientDB::ReadWorldMapAreas()
{
	DB2Reader reader = DB2Reader(string("D:\\Bot\\Extracted\\dbfilesclient\\worldmaparea.db2"),true);
	auto records = reader.ReadWhole<WorldMapAreaRec>();
	vector<WorldMapArea> result;
	for (auto & r : records)
	{
		WorldMapArea a(r, string(reader.GetStrings() + r.name));
		result.push_back(move(a));
	}
	return result;
}
