#include "stdafx.h"
#include <algorithm>
NavConfig SquareArea::config;
SquareArea::SquareArea()
{
}
SquareArea::SquareArea(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius) :location(location), block_coordinates(block_coordinates), coordinates(coordinates), radius(radius)
{
	m_PathStore = unique_ptr<PATHDATA>(new PATHDATA);
	polys = vector<unique_ptr<rcPolyMesh>>();
	m_navMesh = 0;
	busy = false;
	area_size = radius * 2 + 1;
	chunks = new Chunk**[area_size];
	/*doodads = vector<Doodad*>();
	to_update = true;
	old_doodads = vector<Doodad*>();
	active_doodads = vector<Doodad*>();
	wmos = vector<WMO*>();
	old_wmos = vector<WMO*>();
	active_wmos = vector<WMO*>();*/
	wow_object_avatars = vector<WowObjectAvatar*>();
	for (int i = 0; i < area_size; i++)
	{
		chunks[i] = new Chunk *[area_size];
		for (int j = 0; j < area_size; j++)
		{
			chunks[i][j] = 0;
		}
	}
	Fill(location, block_coordinates, coordinates);
	/*InitObjects();
	InitActiveObjects();*/
	//InitBoundingBox();
	//navigation.SetArea(*this);
}


SquareArea::~SquareArea(void)
{

}

SquareArea & SquareArea::operator=(SquareArea && right)
{
	
	chunks = right.chunks;
	radius = right.radius;
	/*doodads = move(right.doodads);
	wmos = move(right.wmos);
	old_wmos = move(right.old_wmos);
	old_doodads = move(right.old_doodads);
	active_wmos = move(right.active_wmos);
	active_doodads = move(right.active_doodads);*/
	area_size = right.area_size;
	location = right.location;
	block_coordinates = right.block_coordinates;
	coordinates = right.coordinates;
	bounding_box = right.bounding_box;
	to_update = right.to_update;
	busy = right.busy;
	m_PathStore = move(right.m_PathStore);
	polys = move(right.polys);
	m_navQuery = right.m_navQuery;
	right.m_navQuery = nullptr;
	m_navMesh = right.m_navMesh;
	right.m_navMesh = nullptr;

	return *this;
}

void SquareArea::Fill(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates)
{
	this->location = location;
	this->block_coordinates = block_coordinates;
	this->coordinates = coordinates;
	int area_size = radius * 2 + 1;
	Point2D<int> area_position = (coordinates + Point2D<int>(16, 16)) - Point2D<int>(radius, radius);
	Vector3 block_real_position = Vector3(block_coordinates.Y*Metrics::BlockSize, -block_coordinates.X*Metrics::BlockSize, 0.0f);
	int color = 10;

	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			Point2D<int> abs_pos = area_position + Point2D<int>(i, j);

			chunks[i][j] = ADTWorker::GetChunk(location, block_coordinates += (Point2D<int>(abs_pos.X / 16, abs_pos.Y / 16) - Point2D<int>(1, 1)), Point2D<int>(abs_pos.X % 16, abs_pos.Y % 16), true);
			if (!chunks[i][j]) continue;
			chunks[i][j]->InitNavigation();
			
		}

	}
	DeleteDuplicates();
	//InitBoundingBox();
	InitNavigation();
//	TestNav();
	ToMesh();


}

void SquareArea::CheckAndMove(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates)
{

	if (this->location->id != location->id || this->block_coordinates != block_coordinates || this->coordinates != coordinates)
	{
		if (!busy)
		{
			busy = true;
			bool block_changed = false;
			if (this->location->id != location->id || this->block_coordinates != block_coordinates)
				block_changed = true;
			Fill(location, block_coordinates, coordinates);
			//if (block_changed)
			/*	InitObjects();
			InitActiveObjects();*/
			//InitBoundingBox();
			//navigation.SetArea(*this);
			to_update = true;
			busy = false;
		}

	}

}
//void SquareArea::InitObjects()
//{
//
//	InitDoodads();
//	InitWMOs();
//
//}
void SquareArea::AddWowObjectAvatar(Wow::WowObject * object)
{
	WowObjectAvatar * avatar = new WowObjectAvatar(object);
	/*
	Position pos=object->GetPosition();
	pos.coords=Vector3(Metrics::MapMidPoint - pos.coords.y,-(Metrics::MapMidPoint - pos.coords.x),pos.coords.z);
	M2 * m2 = new M2("E:\\Extracted\\Character\\Tauren\\Male\\TaurenMale.M2");
	Doodad * doodad=new Doodad(m2,0,pos,1024);
	//Doodad * doodad=new Doodad(m2,0,object->GetPosition(),1024);
	//real_position=Vector3(Metrics::MapMidPoint - header.position.y,-(Metrics::MapMidPoint - header.position.x),header.position.z);
	doodad->SetPosition(pos);
	for (unsigned long i=0;i<doodad->GetVertexCount();i++)
	{
	doodad->GetVertices()[i].position.x*=100;
	doodad->GetVertices()[i].position.y*=100;
	doodad->GetVertices()[i].position.z*=100;
	}
	delete m2;
	*/
	wow_object_avatars.push_back(avatar);
}
//void SquareArea::InitActiveObjects()
//{
//
//	
//	active_doodads.clear();
//	for (auto doodad : doodads)
//	{
//		//if (bounding_box.IsInside2D(doodad->GetPosition().coords))
//		{
//			active_doodads.push_back(doodad);
//		}
//	}
//	
//	active_wmos.clear();
//	for (auto &wmo : wmos)
//	{
//		//if (bounding_box.IsInside2D(wmo->GetPosition().coords))
//		{
//			active_wmos.push_back(wmo);
//		}
//	}
//}


void SquareArea::DeleteDuplicates()
{
	vector<unsigned> wmo_uuids = vector<unsigned>();
	vector<unsigned> doodad_uuids = vector<unsigned>();
	bool exist = false;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			if (chunks[i][j])
				for (auto & wmo : chunks[i][j]->GetWMOs())
				{
					for (auto uuid : wmo_uuids)
					{
						if (uuid == wmo.GetUUID())
						{
							wmo.Skip();
							break;
						}
					}
					if (wmo.IsSkipped())
						continue;
					wmo_uuids.push_back(wmo.GetUUID());

				}
			for (auto & doodad : chunks[i][j]->GetDoodads())
			{
				for (auto uuid :doodad_uuids)
				{
					if (uuid == doodad.GetUUID())
					{
						doodad.Skip();
						break;
					}
				}
				if (doodad.IsSkipped())
					continue;
				doodad_uuids.push_back(doodad.GetUUID());

			}
		}
	}
}
void SquareArea::InitNavigation()
{
	InitAreaBoundingBox();
	BuildAllTiles();
	
}

void SquareArea::InitAreaBoundingBox()
{
	float  hmin = numeric_limits<float>::min();
	float  hmax = numeric_limits<float>::min();
	vector<float> points = vector<float>();
	Chunk * chunk;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			chunk = chunks[i][j];
			if (chunk)
			{

				points.push_back(chunk->GetTerrainBoundingBox().up.x);
				points.push_back(chunk->GetFullBoundingBox().up.y);
				points.push_back(chunk->GetTerrainBoundingBox().up.z);
				points.push_back(chunk->GetTerrainBoundingBox().down.x);
				points.push_back(chunk->GetFullBoundingBox().down.y);
				points.push_back(chunk->GetTerrainBoundingBox().down.z);

			}
			
		}
	}
	rcCalcBounds(&points[0], points.size() / 3, bounding_box.GetArrayMin(), bounding_box.GetArrayMax());

}

void SquareArea::BuildAllTiles()
{
	polys.clear();
	m_navQuery = new dtNavMeshQuery();
	unique_ptr<rcContext> m_ctx_ptr = unique_ptr<rcContext>(new rcContext());
	rcContext * m_ctx = m_ctx_ptr.get();
	//rcContext *m_ctx = new rcContext();
	const float* bmin = bounding_box.GetArrayMin();
	const float* bmax = bounding_box.GetArrayMax();
	int gw = 0, gh = 0;
	rcCalcGridSize(bmin, bmax, config.m_cellSize, &gw, &gh);
	const int ts = (int)config.m_tileSize;
	const int tw = (gw + ts - 1) / ts;
	const int th = (gh + ts - 1) / ts;
	const float tcs = config.m_tileSize*config.m_cellSize;
	float m_tileBmin[3];
	float m_tileBmax[3];
	dtFreeNavMesh(m_navMesh);

	m_navMesh = dtAllocNavMesh();
	if (!m_navMesh)
	{
		//m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not allocate navmesh.");
		return;
	}
	dtNavMeshParams params;
	rcVcopy(params.orig, bmin);
	params.tileWidth = config.m_tileSize*config.m_cellSize;
	params.tileHeight = config.m_tileSize*config.m_cellSize;
	params.maxTiles = config.m_maxTiles;
	params.maxPolys = config.m_maxPolysPerTile;

	dtStatus status;
	//m_navMesh = new dtNavMesh();
	status = m_navMesh->init(&params);
	if (dtStatusFailed(status))
	{
		//m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init navmesh.");
		return;
	}

	status = m_navQuery->init(m_navMesh, 2048);
	if (dtStatusFailed(status))
	{
		//m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init Detour navmesh query");
		return;
	}
	int count = 0;
	for (int y = 0; y < th; ++y)
	{
		for (int x = 0; x < tw; ++x)
		{
			m_tileBmin[0] = bmin[0] + x*tcs;
			m_tileBmin[1] = bmin[1];
			m_tileBmin[2] = bmin[2] + y*tcs;

			m_tileBmax[0] = bmin[0] + (x + 1)*tcs;
			m_tileBmax[1] = bmax[1];
			m_tileBmax[2] = bmin[2] + (y + 1)*tcs;

			int dataSize = 0;
			unsigned char* data = BuildTileMesh(x, y, m_tileBmin, m_tileBmax, dataSize);
			
			if (!data) count++;
			if (data)
			{
				// Remove any previous data (navmesh owns and deletes the data).
				m_navMesh->removeTile(m_navMesh->getTileRefAt(x, y, 0), 0, 0);
				// Let the navmesh own the data.
				dtStatus status = m_navMesh->addTile(data, dataSize, DT_TILE_FREE_DATA, 0, 0);
				if (dtStatusFailed(status))
					dtFree(data);
			}
		}
	}
	status = m_navQuery->init(m_navMesh, 2048);
	if (dtStatusFailed(status))
	{
		//m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init Detour navmesh query");
		return;
	}
}

unsigned char * SquareArea::BuildTileMesh(int x, int y, const float* bmin, const float* bmax, int dataSize)
{
	unique_ptr<rcContext> m_ctx_ptr = unique_ptr<rcContext>(new rcContext());
	rcContext * m_ctx = m_ctx_ptr.get();
	//rcContext * m_ctx = new rcContext();
	rcConfig m_cfg;
	std::memset(&m_cfg, 0, sizeof(m_cfg));
	m_cfg.cs = config.m_cellSize;
	m_cfg.ch = config.m_cellHeight;
	m_cfg.walkableSlopeAngle = config.m_agentMaxSlope;
	m_cfg.walkableHeight = (int)ceilf(config.m_agentHeight / m_cfg.ch);
	m_cfg.walkableClimb = (int)floorf(config.m_agentMaxClimb / m_cfg.ch);
	m_cfg.walkableRadius = (int)ceilf(config.m_agentRadius / m_cfg.cs);
	m_cfg.maxEdgeLen = (int)(config.m_edgeMaxLen / config.m_cellSize);
	m_cfg.maxSimplificationError = config.m_edgeMaxError;
	m_cfg.minRegionArea = (int)rcSqr(config.m_regionMinSize);		// Note: area = size*size
	m_cfg.mergeRegionArea = (int)rcSqr(config.m_regionMergeSize);	// Note: area = size*size
	m_cfg.maxVertsPerPoly = (int)config.m_vertsPerPoly;
	m_cfg.tileSize = (int)config.m_tileSize;
	m_cfg.borderSize = m_cfg.walkableRadius + 3; // Reserve enough padding.
	m_cfg.width = m_cfg.tileSize + m_cfg.borderSize * 2;
	m_cfg.height = m_cfg.tileSize + m_cfg.borderSize * 2;
	m_cfg.detailSampleDist = config.m_detailSampleDist < 0.9f ? 0 : config.m_cellSize * config.m_detailSampleDist;
	m_cfg.detailSampleMaxError = config.m_cellHeight * config.m_detailSampleMaxError;

	rcVcopy(m_cfg.bmin, bmin);
	rcVcopy(m_cfg.bmax, bmax);
	m_cfg.bmin[0] -= m_cfg.borderSize*m_cfg.cs;
	m_cfg.bmin[2] -= m_cfg.borderSize*m_cfg.cs;
	m_cfg.bmax[0] += m_cfg.borderSize*m_cfg.cs;
	m_cfg.bmax[2] += m_cfg.borderSize*m_cfg.cs;

	unique_ptr<rcHeightfield> m_solid_ptr = unique_ptr<rcHeightfield>(rcAllocHeightfield());
	//rcHeightfield * m_solid = rcAllocHeightfield();
	rcHeightfield * m_solid = m_solid_ptr.get();
	if (!m_solid)
	{
		//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'solid'.");
		return 0;
	}
	if (!rcCreateHeightfield(m_ctx, *m_solid, m_cfg.width, m_cfg.height, m_cfg.bmin, m_cfg.bmax, m_cfg.cs, m_cfg.ch))
	{
		//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create solid heightfield.");
		return 0;
	}

	float tbmin[2], tbmax[2];
	tbmin[0] = m_cfg.bmin[0];
	tbmin[1] = m_cfg.bmin[2];
	tbmax[0] = m_cfg.bmax[0];
	tbmax[1] = m_cfg.bmax[2];
	vector<Chunk*> overlapping_chunks = vector<Chunk*>();
	unique_ptr<unsigned char>m_triareas_ptr;
	unsigned char * m_triareas;
	int m_tileTriCount = 0;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			Chunk * chunk = chunks[i][j];
			if (!chunk)
				continue;
			Utils::Graphics::BoundingBox & bbox = chunk->GetTerrainBoundingBox();
			bool overlap = true;
			overlap = (tbmin[0] > bbox.down.x || tbmax[0] < bbox.up.x) ? false : overlap;
			overlap = (tbmin[1] > bbox.down.z || tbmax[1] < bbox.up.z) ? false : overlap;
			if (overlap)
			{
				overlapping_chunks.push_back(chunk);
				float * verts = chunk->nav_vertices.get();
				const int* ctris = chunk->nav_indices.get();
				const int nctris = chunk->ind_count / 3;
				m_tileTriCount += nctris;
				m_triareas_ptr.reset();
				m_triareas_ptr = unique_ptr<unsigned char>(new unsigned char[nctris]);
				m_triareas = m_triareas_ptr.get();
				//m_triareas = new unsigned char[nctris];
				rcMarkWalkableTriangles(m_ctx, m_cfg.walkableSlopeAngle, verts, chunk->vert_count, ctris, nctris, m_triareas);
				if (!rcRasterizeTriangles(m_ctx, verts, chunk->vert_count, ctris, m_triareas, nctris, *m_solid, m_cfg.walkableClimb))
					return 0;
				//delete[] m_triareas;
				m_triareas = 0;
			}

		}
	}
	rcFilterLowHangingWalkableObstacles(m_ctx, m_cfg.walkableClimb, *m_solid);
	rcFilterLedgeSpans(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid);
	rcFilterWalkableLowHeightSpans(m_ctx, m_cfg.walkableHeight, *m_solid);

	unique_ptr<rcCompactHeightfield> m_chf_ptr = unique_ptr<rcCompactHeightfield>(rcAllocCompactHeightfield());
	rcCompactHeightfield * m_chf = m_chf_ptr.get();
	//rcCompactHeightfield * m_chf = rcAllocCompactHeightfield();
	if (!m_chf)
	{
		//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'chf'.");
		return 0;
	}
	if (!rcBuildCompactHeightfield(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid, *m_chf))
	{
		//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build compact data.");
		return 0;
	}

	//rcFreeHeightField(m_solid);
	m_solid = 0;

	if (!rcErodeWalkableArea(m_ctx, m_cfg.walkableRadius, *m_chf))
	{
		//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not erode.");
		return 0;
	}

	if (config.m_partitionType == SAMPLE_PARTITION_WATERSHED)
	{
		// Prepare for region partitioning, by calculating distance field along the walkable surface.
		if (!rcBuildDistanceField(m_ctx, *m_chf))
		{
			//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build distance field.");
			return false;
		}

		// Partition the walkable surface into simple regions without holes.
		if (!rcBuildRegions(m_ctx, *m_chf, m_cfg.borderSize, m_cfg.minRegionArea, m_cfg.mergeRegionArea))
		{
			//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build watershed regions.");
			return false;
		}
	}
	unique_ptr<rcContourSet> m_cset_ptr= unique_ptr<rcContourSet>(rcAllocContourSet());
	rcContourSet * m_cset = m_cset_ptr.get();
	//rcContourSet * m_cset = rcAllocContourSet();
	if (!m_cset)
	{
		//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'cset'.");
		return 0;
	}
	if (!rcBuildContours(m_ctx, *m_chf, m_cfg.maxSimplificationError, m_cfg.maxEdgeLen, *m_cset))
	{
		//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create contours.");
		return 0;
	}

	if (m_cset->nconts == 0)
	{
		return 0;
	}

	// Build polygon navmesh from the contours.
	unique_ptr<rcPolyMesh> m_pmesh_ptr = unique_ptr<rcPolyMesh>(rcAllocPolyMesh());
	rcPolyMesh* m_pmesh = m_pmesh_ptr.get();
	//rcPolyMesh *  m_pmesh = rcAllocPolyMesh();
	if (!m_pmesh)
	{
		//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmesh'.");
		return 0;
	}
	
	if (!rcBuildPolyMesh(m_ctx, *m_cset, m_cfg.maxVertsPerPoly, *m_pmesh))
	{
		//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not triangulate contours.");
		return 0;
	}
	polys.push_back(move(m_pmesh_ptr));
	// Build detail mesh.
	unique_ptr<rcPolyMeshDetail> m_dmesh_ptr = unique_ptr<rcPolyMeshDetail>(rcAllocPolyMeshDetail());
	rcPolyMeshDetail* m_dmesh = m_dmesh_ptr.get();
	//rcPolyMeshDetail * m_dmesh = rcAllocPolyMeshDetail();
	if (!m_dmesh)
	{
		//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'dmesh'.");
		return 0;
	}

	if (!rcBuildPolyMeshDetail(m_ctx, *m_pmesh, *m_chf,m_cfg.detailSampleDist, m_cfg.detailSampleMaxError,*m_dmesh))
	{
		//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could build polymesh detail.");
		return 0;
	}
	m_chf = 0;
	m_cset = 0;

	unsigned char* navData = 0;
	int navDataSize = 0;
	if (m_cfg.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
	{
		if (m_pmesh->nverts >= 0xffff)
		{
			// The vertex indices are ushorts, and cannot point to more than 0xffff vertices.
			//m_ctx->log(RC_LOG_ERROR, "Too many vertices per tile %d (max: %d).", m_pmesh->nverts, 0xffff);
			return 0;
		}

		// Update poly flags from areas.
		for (int i = 0; i < m_pmesh->npolys; ++i)
		{
			if (m_pmesh->areas[i] == RC_WALKABLE_AREA)
				m_pmesh->areas[i] = SAMPLE_POLYAREA_GROUND;

			if (m_pmesh->areas[i] == SAMPLE_POLYAREA_GROUND ||
				m_pmesh->areas[i] == SAMPLE_POLYAREA_GRASS ||
				m_pmesh->areas[i] == SAMPLE_POLYAREA_ROAD)
			{
				m_pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK;
			}
			else if (m_pmesh->areas[i] == SAMPLE_POLYAREA_WATER)
			{
				m_pmesh->flags[i] = SAMPLE_POLYFLAGS_SWIM;
			}
			else if (m_pmesh->areas[i] == SAMPLE_POLYAREA_DOOR)
			{
				m_pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
			}
		}

		dtNavMeshCreateParams params;
		std::memset(&params, 0, sizeof(params));
		params.verts = m_pmesh->verts;
		params.vertCount = m_pmesh->nverts;
		params.polys = m_pmesh->polys;
		params.polyAreas = m_pmesh->areas;
		params.polyFlags = m_pmesh->flags;
		params.polyCount = m_pmesh->npolys;
		params.nvp = m_pmesh->nvp;
		params.detailMeshes = m_dmesh->meshes;
		params.detailVerts = m_dmesh->verts;
		params.detailVertsCount = m_dmesh->nverts;
		params.detailTris = m_dmesh->tris;
		params.detailTriCount = m_dmesh->ntris;
		params.offMeshConVerts =0;
		params.offMeshConRad = 0;
		params.offMeshConDir = 0;
		params.offMeshConAreas =0;
		params.offMeshConFlags = 0;
		params.offMeshConUserID = 0;
		params.offMeshConCount = 0;
		params.walkableHeight = config.m_agentHeight;
		params.walkableRadius = config.m_agentRadius;
		params.walkableClimb = config.m_agentMaxClimb;
		params.tileX = x;
		params.tileY = y;
		params.tileLayer = 0;
		rcVcopy(params.bmin, m_pmesh->bmin);
		rcVcopy(params.bmax, m_pmesh->bmax);
		params.cs = m_cfg.cs;
		params.ch = m_cfg.ch;
		params.buildBvTree = true;

		if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
		{
			//m_ctx->log(RC_LOG_ERROR, "Could not build Detour navmesh.");
			return 0;
		}
	}
	dataSize = navDataSize;
	return navData;
}

int SquareArea::FindPath(Vector3 & start, Vector3 & end, int nPathSlot)
{
	
	//float m_smoothPath[MAX_SMOOTH * 3];
	const int MAX_POLYS = 256;
	//int m_nsmoothPath = 0;
	float * m_spos = (float*)&start;
	float * m_epos = (float*)&end;
	dtStatus status;
	float pExtents[3] = { 2.0f, 4.0f, 2.0f }; // size of box around start/end points to look for nav polygons
	dtPolyRef m_startRef;
	float StartNearest[3];
	dtPolyRef m_endRef;
	float EndNearest[3];
	dtPolyRef m_polys[MAX_PATHPOLY];
	int m_npolys = 0;
	float StraightPath[MAX_PATHVERT * 3];
	int nVertCount = 0;
	PATHDATA * path_store = m_PathStore.get();

	// setup the filter
	dtQueryFilter m_filter;
	m_filter.setIncludeFlags(0xFFFF);
	m_filter.setExcludeFlags(0);
	m_filter.setAreaCost(SAMPLE_POLYAREA_GROUND, 1.0f);

	// find the start polygon
	status = m_navQuery->findNearestPoly((float*)&start, pExtents, &m_filter, &m_startRef, 0);
	if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK)) 
		return -1; // couldn't find a polygon

																		  // find the end polygon
	status = m_navQuery->findNearestPoly((float*)&end, pExtents, &m_filter, &m_endRef, 0);
	if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK)) 
		return -2; // couldn't find a polygon

	if (m_startRef && m_endRef)
	{
		m_navQuery->findPath(m_startRef, m_endRef, m_spos, m_epos, &m_filter, m_polys, &m_npolys, MAX_POLYS);

		m_nsmoothPath = 0;
		if (m_npolys)
		{
			dtPolyRef polys[MAX_POLYS];
			memcpy(polys, m_polys, sizeof(dtPolyRef)*m_npolys);
			int npolys = m_npolys;

			float iterPos[3], targetPos[3];
			m_navQuery->closestPointOnPoly(m_startRef, m_spos, iterPos, 0);
			m_navQuery->closestPointOnPoly(polys[npolys - 1], m_epos, targetPos, 0);

			static const float STEP_SIZE = 0.5f;
			static const float SLOP = 0.01f;

			m_nsmoothPath = 0;

			dtVcopy(&m_smoothPath[m_nsmoothPath * 3], iterPos);
			m_nsmoothPath++;
			cout << m_nsmoothPath << endl;

			while (npolys && m_nsmoothPath < MAX_SMOOTH)
			{
				// Find location to steer towards.
				float steerPos[3];
				unsigned char steerPosFlag;
				dtPolyRef steerPosRef;

				if (!getSteerTarget(m_navQuery, iterPos, targetPos, SLOP,
					polys, npolys, steerPos, steerPosFlag, steerPosRef))
					break;

				bool endOfPath = (steerPosFlag & DT_STRAIGHTPATH_END) ? true : false;
				bool offMeshConnection = (steerPosFlag & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ? true : false;

				// Find movement delta.
				float delta[3], len;
				dtVsub(delta, steerPos, iterPos);
				len = dtMathSqrtf(dtVdot(delta, delta));
				// If the steer target is end of path or off-mesh link, do not move past the location.
				if ((endOfPath || offMeshConnection) && len < STEP_SIZE)
					len = 1;
				else
					len = STEP_SIZE / len;
				float moveTgt[3];
				dtVmad(moveTgt, iterPos, delta, len);

				// Move
				float result[3];
				dtPolyRef visited[16];
				int nvisited = 0;
				m_navQuery->moveAlongSurface(polys[0], iterPos, moveTgt, &m_filter,
					result, visited, &nvisited, 16);

				npolys = fixupCorridor(polys, npolys, MAX_POLYS, visited, nvisited);
				npolys = fixupShortcuts(polys, npolys, m_navQuery);

				float h = 0;
				m_navQuery->getPolyHeight(polys[0], result, &h);
				result[1] = h;
				dtVcopy(iterPos, result);

				// Handle end of path and off-mesh links when close enough.
				if (endOfPath && inRange(iterPos, steerPos, SLOP, 1.0f))
				{
					// Reached end of path.
					dtVcopy(iterPos, targetPos);
					if (m_nsmoothPath < MAX_SMOOTH)
					{
						dtVcopy(&m_smoothPath[m_nsmoothPath * 3], iterPos);
						m_nsmoothPath++;
					}
					break;
				}
				else if (offMeshConnection && inRange(iterPos, steerPos, SLOP, 1.0f))
				{
					// Reached off-mesh connection.
					float startPos[3], endPos[3];

					// Advance the path up to and over the off-mesh connection.
					dtPolyRef prevRef = 0, polyRef = polys[0];
					int npos = 0;
					while (npos < npolys && polyRef != steerPosRef)
					{
						prevRef = polyRef;
						polyRef = polys[npos];
						npos++;
					}
					for (int i = npos; i < npolys; ++i)
						polys[i - npos] = polys[i];
					npolys -= npos;

					// Handle the connection.
					dtStatus status = m_navMesh->getOffMeshConnectionPolyEndPoints(prevRef, polyRef, startPos, endPos);
					if (dtStatusSucceed(status))
					{
						if (m_nsmoothPath < MAX_SMOOTH)
						{
							dtVcopy(&m_smoothPath[m_nsmoothPath * 3], startPos);
							m_nsmoothPath++;
							// Hack to make the dotted path not visible during off-mesh connection.
							if (m_nsmoothPath & 1)
							{
								dtVcopy(&m_smoothPath[m_nsmoothPath * 3], startPos);
								m_nsmoothPath++;
							}
						}
						// Move position at the other side of the off-mesh link.
						dtVcopy(iterPos, endPos);
						float eh = 0.0f;
						m_navQuery->getPolyHeight(polys[0], iterPos, &eh);
						iterPos[1] = eh;
					}
				}

				// Store results.
				if (m_nsmoothPath < MAX_SMOOTH)
				{
					dtVcopy(&m_smoothPath[m_nsmoothPath * 3], iterPos);
					m_nsmoothPath++;
				}
			}

		}
	}


	//status = m_navQuery->findPath(StartPoly, EndPoly, StartNearest, EndNearest, &Filter, PolyPath, &nPathCount, MAX_PATHPOLY);
	//if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK)) return -3; // couldn't create a path
	//if (nPathCount == 0) 
	//	return -4; // couldn't find a path

	//status = m_navQuery->findStraightPath(StartNearest, EndNearest, PolyPath, nPathCount, StraightPath, NULL, NULL, &nVertCount, MAX_PATHVERT + 100);
	//if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK)) 
	//	return -5; // couldn't create a path
	//if (nVertCount == 0) 
	//	return -6; // couldn't find a path

									// At this point we have our path.  Copy it to the path store
	/*int nIndex = 0;
	for (int nVert = 0; nVert<nVertCount; nVert++)
	{
		path_store->PosX[nVert] = StraightPath[nIndex++];
		path_store->PosY[nVert] = StraightPath[nIndex++];
		path_store->PosZ[nVert] = StraightPath[nIndex++];
	}
	path_store->MaxVertex = nVertCount;*/
	//m_PathStore[nPathSlot].Target = nTarget;
	return m_nsmoothPath;
}


bool SquareArea::getSteerTarget(dtNavMeshQuery* navQuery, const float* startPos, const float* endPos,
	const float minTargetDist,
	const dtPolyRef* path, const int pathSize,
	float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef,
	float* outPoints, int* outPointCount)
{
	// Find steer target.
	static const int MAX_STEER_POINTS = 3;
	float steerPath[MAX_STEER_POINTS * 3];
	unsigned char steerPathFlags[MAX_STEER_POINTS];
	dtPolyRef steerPathPolys[MAX_STEER_POINTS];
	int nsteerPath = 0;
	navQuery->findStraightPath(startPos, endPos, path, pathSize,
		steerPath, steerPathFlags, steerPathPolys, &nsteerPath, MAX_STEER_POINTS);
	if (!nsteerPath)
		return false;

	if (outPoints && outPointCount)
	{
		*outPointCount = nsteerPath;
		for (int i = 0; i < nsteerPath; ++i)
			dtVcopy(&outPoints[i * 3], &steerPath[i * 3]);
	}


	// Find vertex far enough to steer to.
	int ns = 0;
	while (ns < nsteerPath)
	{
		// Stop at Off-Mesh link or when point is further than slop away.
		if ((steerPathFlags[ns] & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ||
			!inRange(&steerPath[ns * 3], startPos, minTargetDist, 1000.0f))
			break;
		ns++;
	}
	// Failed to find good point to steer to.
	if (ns >= nsteerPath)
		return false;

	dtVcopy(steerPos, &steerPath[ns * 3]);
	steerPos[1] = startPos[1];
	steerPosFlag = steerPathFlags[ns];
	steerPosRef = steerPathPolys[ns];

	return true;
}
inline bool SquareArea::inRange(const float* v1, const float* v2, const float r, const float h)
{
	const float dx = v2[0] - v1[0];
	const float dy = v2[1] - v1[1];
	const float dz = v2[2] - v1[2];
	return (dx*dx + dz*dz) < r*r && fabsf(dy) < h;
}
int SquareArea::fixupShortcuts(dtPolyRef* path, int npath, dtNavMeshQuery* navQuery)
{
	if (npath < 3)
		return npath;

	// Get connected polygons
	static const int maxNeis = 16;
	dtPolyRef neis[maxNeis];
	int nneis = 0;

	const dtMeshTile* tile = 0;
	const dtPoly* poly = 0;
	if (dtStatusFailed(navQuery->getAttachedNavMesh()->getTileAndPolyByRef(path[0], &tile, &poly)))
		return npath;

	for (unsigned int k = poly->firstLink; k != DT_NULL_LINK; k = tile->links[k].next)
	{
		const dtLink* link = &tile->links[k];
		if (link->ref != 0)
		{
			if (nneis < maxNeis)
				neis[nneis++] = link->ref;
		}
	}

	// If any of the neighbour polygons is within the next few polygons
	// in the path, short cut to that polygon directly.
	static const int maxLookAhead = 6;
	int cut = 0;
	for (int i = dtMin(maxLookAhead, npath) - 1; i > 1 && cut == 0; i--) {
		for (int j = 0; j < nneis; j++)
		{
			if (path[i] == neis[j]) {
				cut = i;
				break;
			}
		}
	}
	if (cut > 1)
	{
		int offset = cut - 1;
		npath -= offset;
		for (int i = 1; i < npath; i++)
			path[i] = path[i + offset];
	}

	return npath;
}


int SquareArea::fixupCorridor(dtPolyRef* path, const int npath, const int maxPath,
	const dtPolyRef* visited, const int nvisited)
{
	int furthestPath = -1;
	int furthestVisited = -1;

	// Find furthest common polygon.
	for (int i = npath - 1; i >= 0; --i)
	{
		bool found = false;
		for (int j = nvisited - 1; j >= 0; --j)
		{
			if (path[i] == visited[j])
			{
				furthestPath = i;
				furthestVisited = j;
				found = true;
			}
		}
		if (found)
			break;
	}

	// If no intersection found just return current path. 
	if (furthestPath == -1 || furthestVisited == -1)
		return npath;

	// Concatenate paths.	

	// Adjust beginning of the buffer to include the visited.
	const int req = nvisited - furthestVisited;
	const int orig = rcMin(furthestPath + 1, npath);
	int size = rcMax(0, npath - orig);
	if (req + size > maxPath)
		size = maxPath - req;
	if (size)
		memmove(path + req, path + orig, size*sizeof(dtPolyRef));

	// Store visited
	for (int i = 0; i < req; ++i)
		path[i] = visited[(nvisited - 1) - i];

	return req + size;
}



void SquareArea::InitNavConfig()
{
	config.m_maxTiles = 1024;
	config.m_maxPolysPerTile = 4096;
	config.m_tileSize = 32.0;
	config.m_cellSize = 0.3;
	config.m_cellHeight = 0.2;
	config.m_agentHeight = 2.0;
	config.m_agentRadius =  0.600000024;
	config.m_agentMaxClimb = 0.899999976;
	config.m_agentMaxSlope = 45.0000000;
	config.m_regionMinSize = 8.00000000;
	config.m_regionMergeSize = 20.0000000;
	config.m_edgeMaxLen = 12.0000000;
	config.m_edgeMaxError = 1.29999995;
	config.m_vertsPerPoly = 6.00000000;
	config.m_detailSampleDist = 6.00000000;
	config.m_detailSampleMaxError = 1.00000000;
	config.m_partitionType = 0;
}
void SquareArea::ToMesh()
{
	unsigned ind_count = 0;
	unsigned vert_count = 0;
	
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			Chunk * chunk = chunks[i][j];
			if (!chunk) continue;
			vert_count += chunk->vert_count;
			ind_count += chunk->ind_count;
		}
		
	}
	float * vertices = new float[vert_count * 3];
	int * indices = new int[ind_count];
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
		}
	}
	int vert_offset = 0;
	ofstream file;
	file.open("new test.obj");
	/*for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			Chunk * chunk = chunks[i][j];
			if (!chunk) continue;
			for (int vi = 0; vi < chunk->vert_count * 3; vi += 3)
			{
				vertices[vert_offset] = chunk->nav_vertices[vi];
				vertices[vert_offset+1] = chunk->nav_vertices[vi+1];
				vertices[vert_offset+2] = chunk->nav_vertices[vi+2];
			}
			for (int ii = 0; ii < chunk->ind_count * 3; ii += 3)
			{
				vertices[vert_offset] = chunk->nav_vertices[ii];
				vertices[vert_offset + 1] = chunk->nav_vertices[ii + 1];
				vertices[vert_offset + 2] = chunk->nav_vertices[ii + 2];
			}
		}
	}*/

	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			Chunk * chunk = chunks[i][j];
			if (!chunk) continue;
			for (int vi = 0; vi < chunk->vert_count*3; vi+=3)
			{
				file << "v  "<< chunk->nav_vertices.get()[vi] << " " << chunk->nav_vertices.get()[vi + 1] << " " << chunk->nav_vertices.get()[vi + 2] << endl;
			}
		}
	}
	vert_offset = 0;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			Chunk * chunk = chunks[i][j];
			if (!chunk) continue;
			for (int ii = 0; ii < chunk->ind_count ; ii += 3)
			{
				file << "f  "<< chunk->nav_indices.get()[ii]+vert_offset +1<< " " << chunk->nav_indices.get()[ii+1]+ vert_offset+1 << " " << chunk->nav_indices.get()[ii + 2]+ vert_offset+1 << endl;
			}
			vert_offset += chunk->vert_count;
		}
	}
}
//
//void SquareArea::InitWMOs()
//{
//	wmos.clear();
//	active_wmos.clear();
//	int c = 0;
//	bool exist = false;
//	for (int i = 0; i < area_size; i++)
//	{
//		for (int j = 0; j < area_size; j++)
//		{
//			if (chunks[i][j])
//				for (auto & wmo : chunks[i][j]->GetWMOs())
//				{
//					exist = false;
//					for (auto & check_wmo : wmos)
//					{
//						if (wmo.GetUUID() == check_wmo->GetUUID() && &wmo != check_wmo)
//						{
//							exist = true;
//							break;
//						}
//					}
//					//if (!exist)
//						wmos.push_back(&wmo);	
//					
//				}
//		}
//	}
//}
//
//void SquareArea::InitDoodads()
//{
//	doodads.clear();
//	active_doodads.clear();
//	int c = 0;
//	for (int i = 0; i < area_size; i++)
//	{
//		for (int j = 0; j < area_size; j++)
//		{
//			if (chunks[i][j])
//				for (auto & doodad : chunks[i][j]->GetDoodads())
//				{
//					doodads.push_back(&doodad);
//				}
//		}
//	}
//}
