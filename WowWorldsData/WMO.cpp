#include "stdafx.h"
#include "WMO.h"


WMO::WMO(WMORoot & root, unsigned uuid, Position position)
{
	parts = vector<WMOPart>();
	for (auto &group : root.GetWMOGroups())
	{
		
		parts.emplace_back(group);

	}
}


WMO::~WMO()
{
}
