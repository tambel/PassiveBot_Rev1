#include "stdafx.h"
#include "LocalAreaNavMesh.h"


void LocalAreaNavMesh::InitConfig()
{
	NavMesh::InitConfig();
	config.m_tileSize = 111.0f;
	config.m_cellSize = 0.3f;
}

LocalAreaNavMesh::LocalAreaNavMesh()
{
}

LocalAreaNavMesh::LocalAreaNavMesh(Location & location, Point2DI block, Point2DI coordinates):NavMesh()
{
	InitConfig();
	area = Area(location, block, coordinates, 1);

}


LocalAreaNavMesh::~LocalAreaNavMesh()
{
}

void LocalAreaNavMesh::Update(Location & location, Point2DI block, Point2DI coordinates)
{
	area.CheckAndUpdate(location, block, coordinates);
	Build();
}
