#include "stdafx.h"
#include "GlobalNavMesh.h"

inline unsigned int ilog2(unsigned int v)
{
	unsigned int r;
	unsigned int shift;
	r = (v > 0xffff) << 4; v >>= r;
	shift = (v > 0xff) << 3; v >>= shift; r |= shift;
	shift = (v > 0xf) << 2; v >>= shift; r |= shift;
	shift = (v > 0x3) << 1; v >>= shift; r |= shift;
	r |= (v >> 1);
	return r;
}

inline unsigned int nextPow2(unsigned int v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}


GlobalNavMesh::GlobalNavMesh()
{
	poly_meshes = vector<rcPolyMesh*>();
	area = new Area(2);
	bbox.up = Vector3(0.0, 0.0, 0.0);
	bbox.down = Vector3(Metrics::MapSize, 0.0, Metrics::MapSize);
	InitConfig();
	const float* bmin = bbox.GetArrayMin();
	const float* bmax = bbox.GetArrayMax();
	int gw = 0, gh = 0;
	rcCalcGridSize(bmin, bmax, config.m_cellSize, &gw, &gh);
	const int ts = (int)config.m_tileSize;
	const int tw = (gw + ts - 1) / ts;
	const int th = (gh + ts - 1) / ts;
	// Max tiles and max polys affect how the tile IDs are caculated.
	// There are 22 bits available for identifying a tile and a polygon.
	int tileBits = rcMin((int)ilog2(nextPow2(tw*th)), 14);
	if (tileBits > 14) tileBits = 14;
	int polyBits = 22 - tileBits;
	config.m_maxTiles = 1 << tileBits;
	config.m_maxPolysPerTile = 1 << polyBits;
	this->m_ctx= new CTX();
}


GlobalNavMesh::~GlobalNavMesh()
{
	
}

void GlobalNavMesh::InitConfig()
{
	config.m_maxTiles = 1024;
	config.m_maxPolysPerTile = 4096;
	config.m_tileSize = 512.0f;
	config.m_cellSize = 1.0f;
	config.m_cellHeight = 0.2f;
	config.m_agentHeight = 2.0f;
	config.m_agentRadius = 1.0f;//0.800000024;
	config.m_agentMaxClimb = 0.899999976f;
	config.m_agentMaxSlope = 45.0000000f;
	config.m_regionMinSize = 8.00000000f;
	config.m_regionMergeSize = 20.0000000f;
	config.m_edgeMaxLen = 12.0000000f;
	config.m_edgeMaxError = 1.29999995f;
	config.m_vertsPerPoly = 6.00000000f;
	config.m_detailSampleDist = 6.00000000f;
	config.m_detailSampleMaxError = 1.00000000f;
	config.m_partitionType = 0;
}

void GlobalNavMesh::Start(int x, int y)
{
	nav_mesh = dtAllocNavMesh();
	if (!nav_mesh)
	{
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not allocate navmesh.");
		return;
	}

	dtNavMeshParams params;
	rcVcopy(params.orig, reinterpret_cast<float*>(&bbox.up));
	params.tileWidth = config.m_tileSize*config.m_cellSize;
	params.tileHeight = config.m_tileSize*config.m_cellSize;
	params.maxTiles = config.m_maxTiles;
	params.maxPolys = config.m_maxPolysPerTile;

	dtStatus status;

	status = nav_mesh->init(&params);
	if (dtStatusFailed(status))
	{
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init navmesh.");
		return;
	}
	AddTile(Point2DI(x, y));
}

void GlobalNavMesh::AddTile(Point2D<int> coordinates)
{

	float m_tileBmin[3];
	float m_tileBmax[3];
	const float* bmin = bbox.GetArrayMin();
	const float* bmax = bbox.GetArrayMax();
	const float tcs = config.m_tileSize*config.m_cellSize;
	m_tileBmin[0] = bmin[0] + coordinates.X*tcs;
	m_tileBmin[2] = bmin[2] + coordinates.Y*tcs;

	m_tileBmax[0] = bmin[0] + (coordinates.X + 1)*tcs;
	m_tileBmax[2] = bmin[2] + (coordinates.Y + 1)*tcs;
	int dataSize = 0;
	area->UpdateArea(LocationBase::Get("Kalimdor"),AbsPositionToBlockCoords(*reinterpret_cast<Vector3*>(m_tileBmin)), Point2DI(0, 0));
	m_tileBmin[1] = bmin[1] + area->GetBoundingBox().up.y;
	m_tileBmax[1] = bmax[1] + area->GetBoundingBox().down.y;

	//area->ToMesh();
	unsigned char* data = BuildTileMesh(coordinates.X, coordinates.Y, m_tileBmin, m_tileBmax, dataSize);
	if (data)
	{
		// Remove any previous data (navmesh owns and deletes the data).
		nav_mesh->removeTile(nav_mesh->getTileRefAt(coordinates.X, coordinates.Y, 0), 0, 0);
		// Let the navmesh own the data.
		dtStatus status = nav_mesh->addTile(data, dataSize, DT_TILE_FREE_DATA, 0, 0);
		if (dtStatusFailed(status))
			dtFree(data);
	}
	
}
void GlobalNavMesh::Cleanup()
{
	delete[] m_triareas;
	m_triareas = 0;
	rcFreeHeightField(m_solid);
	m_solid = 0;
	rcFreeCompactHeightfield(m_chf);
	m_chf = 0;
	rcFreeContourSet(m_cset);
	m_cset = 0;
	//rcFreePolyMesh(m_pmesh);
	//m_pmesh = 0;
	rcFreePolyMeshDetail(m_dmesh);
	m_dmesh = 0;
}


unsigned char * GlobalNavMesh::BuildTileMesh(int x, int y, const float* bmin, const float* bmax, int & dataSize)
{
	//unique_ptr<rcContext> m_ctx_ptr = unique_ptr<rcContext>(new rcContext());
	//rcContext * m_ctx = m_ctx_ptr.get();
	//rcContext * m_ctx = new rcContext();
	Cleanup();
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

	//unique_ptr<rcHeightfield> m_solid_ptr = unique_ptr<rcHeightfield>(rcAllocHeightfield());
	//rcHeightfield * m_solid = m_solid_ptr.get();
	///rcFreeHeightField(m_solid);
	m_solid = rcAllocHeightfield();

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

	/*float tbmin[2], tbmax[2];
	tbmin[0] = m_cfg.bmin[0];
	tbmin[1] = m_cfg.bmin[2];
	tbmax[0] = m_cfg.bmax[0];
	tbmax[1] = m_cfg.bmax[2];*/


	//vector<Chunk*> overlapping_chunks = vector<Chunk*>();
	//unique_ptr<unsigned char>m_triareas_ptr;
	//unsigned char * m_triareas;
	int m_tileTriCount = 0;
	/*for (int i = 0; i < area_size; i++)
	{
	for (int j = 0; j < area_size; j++)
	{*/
	auto  RasterizeModel = [](Model * model, rcConfig & m_cfg, rcContext * m_ctx, rcHeightfield * m_solid, int & m_tileTriCount)
	{

		float tbmin[2], tbmax[2];
		tbmin[0] = m_cfg.bmin[0];
		tbmin[1] = m_cfg.bmin[2];
		tbmax[0] = m_cfg.bmax[0];
		tbmax[1] = m_cfg.bmax[2];
		unique_ptr<unsigned char>m_triareas_ptr;
		unsigned char * m_triareas;
		Utils::Graphics::BoundingBox & bbox = model->GetBoundingBox();
		bool overlap = true;
		overlap = (tbmin[0] > bbox.down.x || tbmax[0] < bbox.up.x) ? false : overlap;
		overlap = (tbmin[1] > bbox.down.z || tbmax[1] < bbox.up.z) ? false : overlap;
		overlap = true;
		if (overlap)
		{
			//overlapping_chunks.push_back(chunk);
			float * verts = model->vertices;
			const int* ctris = model->GetIndices();
			const int nctris = model->GetIndexCount() / 3;
			m_tileTriCount += nctris;
			//m_triareas_ptr.reset();
			//m_triareas_ptr = unique_ptr<unsigned char>(new unsigned char[nctris]);
			//m_triareas = m_triareas_ptr.get();
			m_triareas = new unsigned char[nctris];
			rcMarkWalkableTriangles(m_ctx, m_cfg.walkableSlopeAngle, verts, model->GetVertexCount(), ctris, nctris, m_triareas);
			if (!rcRasterizeTriangles(m_ctx, verts, model->GetVertexCount(), ctris, m_triareas, nctris, *m_solid, m_cfg.walkableClimb))
				return;
			delete[] m_triareas;
			m_triareas = 0;
		}
	};
	for (auto &chunk : area->GetChunks())
	{
		RasterizeModel(&*chunk, m_cfg, m_ctx, m_solid, m_tileTriCount);
	}
	for (auto &doodad : area->GetDoodads())
	{
		RasterizeModel(&*doodad, m_cfg, m_ctx, m_solid, m_tileTriCount);
	}
	for (auto &wmo : area->GetWMOs())
	{
		RasterizeModel(&*wmo, m_cfg, m_ctx, m_solid, m_tileTriCount);
	}
	rcFilterLowHangingWalkableObstacles(m_ctx, m_cfg.walkableClimb, *m_solid);
	rcFilterLedgeSpans(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid);
	rcFilterWalkableLowHeightSpans(m_ctx, m_cfg.walkableHeight, *m_solid);

	m_chf = rcAllocCompactHeightfield();

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
	//unique_ptr<rcContourSet> m_cset_ptr = unique_ptr<rcContourSet>(rcAllocContourSet());
	//rcContourSet * m_cset = m_cset_ptr.get();
	m_cset = rcAllocContourSet();
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
	//unique_ptr<rcPolyMesh> m_pmesh_ptr = unique_ptr<rcPolyMesh>(rcAllocPolyMesh());
	//rcPolyMesh* m_pmesh = m_pmesh_ptr.get();
	//rcFreePolyMesh(m_pmesh);
	m_pmesh = rcAllocPolyMesh();
	if (!m_pmesh)
	{
		//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmesh'.");
		return 0;
	}
	poly_meshes.push_back(m_pmesh);
	if (!rcBuildPolyMesh(m_ctx, *m_cset, m_cfg.maxVertsPerPoly, *m_pmesh))
	{
		//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not triangulate contours.");
		return 0;
	}
	//polys.push_back(move(m_pmesh_ptr));
	// Build detail mesh.
	//unique_ptr<rcPolyMeshDetail> m_dmesh_ptr = unique_ptr<rcPolyMeshDetail>(rcAllocPolyMeshDetail());
	//rcPolyMeshDetail* m_dmesh = m_dmesh_ptr.get();
	//rcFreePolyMeshDetail(m_dmesh);
	m_dmesh = rcAllocPolyMeshDetail();
	if (!m_dmesh)
	{
		//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'dmesh'.");
		return 0;
	}

	if (!rcBuildPolyMeshDetail(m_ctx, *m_pmesh, *m_chf, m_cfg.detailSampleDist, m_cfg.detailSampleMaxError, *m_dmesh))
	{
		//m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could build polymesh detail.");
		return 0;
	}
	//rcFreeCompactHeightfield(m_chf);
	//m_chf = 0;
	//rcFreeContourSet(m_cset);
	//m_cset = 0;

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
		params.offMeshConVerts = 0;
		params.offMeshConRad = 0;
		params.offMeshConDir = 0;
		params.offMeshConAreas = 0;
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
			delete[] navData;
			return 0;
		}
	}
	//m_dmesh_ptr.release();
	dataSize = navDataSize;
	return navData;
}