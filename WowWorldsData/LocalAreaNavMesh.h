#pragma once
#include "NavMesh.h"
class LocalAreaNavMesh :
	public NavMesh
{
	void InitConfig();
public:
	LocalAreaNavMesh();
	LocalAreaNavMesh(Location & location, Point2DI block, Point2DI coordinates);
	~LocalAreaNavMesh();
	LocalAreaNavMesh(LocalAreaNavMesh &&) = default;
	void Update(Location & location, Point2DI block, Point2DI coordinates);
};

