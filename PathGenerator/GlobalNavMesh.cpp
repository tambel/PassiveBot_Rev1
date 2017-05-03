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
	area = new Area(2, AreaFormat::fBlock);
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
	this->m_ctx= new CTX2();
}


GlobalNavMesh::~GlobalNavMesh()
{
	
}

void GlobalNavMesh::InitConfig()
{
	config.m_maxTiles = 1024;
	config.m_maxPolysPerTile = 4096;
	config.m_tileSize = 256.0f;//111, 11111;// Utils::Metrics::ChunkSize;
	config.m_cellSize = 0.3f;
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
	/*
	config.m_maxTiles = 1024;
	config.m_maxPolysPerTile = 4096;
	config.m_tileSize = 512.0f;
	config.m_cellSize = 0.3f;
	config.m_cellHeight = 0.2f;
	config.m_agentHeight = 2.0f;
	config.m_agentRadius = 1.0f;//0.800000024;
	config.m_agentMaxClimb = 1.25f;//0.899999976f;
	config.m_agentMaxSlope = 45.0000000f;
	config.m_regionMinSize = 8.00000000f;
	config.m_regionMergeSize = 20.0000000f;
	config.m_edgeMaxLen = 12.0000000f;
	config.m_edgeMaxError = 1.29999995f;
	config.m_vertsPerPoly = 6.00000000f;
	config.m_detailSampleDist = 6.00000000f;
	config.m_detailSampleMaxError = 1.00000000f;
	config.m_partitionType = 0;
	*/
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
	//AddTile(Point2DI(x, y));
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
	try
	{
		area->UpdateArea(LocationBase::Get("Kalimdor"), AbsPositionToBlockCoords(*reinterpret_cast<Vector3*>(m_tileBmin)), Point2DI(0, 0));
	}
	catch (EmptyAreaException & e)
	{
		return;
	}
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

bool inRange(const float* v1, const float* v2, const float r, const float h)
{
	const float dx = v2[0] - v1[0];
	const float dy = v2[1] - v1[1];
	const float dz = v2[2] - v1[2];
	return (dx*dx + dz*dz) < r*r && fabsf(dy) < h;
}
bool getSteerTarget(dtNavMeshQuery* navQuery, const float* startPos, const float* endPos,
	const float minTargetDist,
	const dtPolyRef* path, const int pathSize,
	float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef,
	float* outPoints = 0, int* outPointCount = 0)
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

int fixupShortcuts(dtPolyRef* path, int npath, dtNavMeshQuery* navQuery)
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
int fixupCorridor(dtPolyRef* path, const int npath, const int maxPath,
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
		memmove(path + req, path + orig, size * sizeof(dtPolyRef));

	// Store visited
	for (int i = 0; i < req; ++i)
		path[i] = visited[(nvisited - 1) - i];

	return req + size;
}

bool GlobalNavMesh::FindPath(Vector3 & start, Vector3 & end, bool tf)
{
	float m_smoothPath[7000];
	unsigned m_nsmoothPath = 0;
	m_navQuery = dtAllocNavMeshQuery();
	dtStatus status = m_navQuery->init(nav_mesh, 2048*2);
	Vector3 coords;
	Vector3 ucoords;
	if (tf)
	{

		 coords = Vector3(Metrics::MapMidPoint - start.x, start.z, Metrics::MapMidPoint - start.y);
		 ucoords = Vector3(Metrics::MapMidPoint - end.x, end.z, Metrics::MapMidPoint - end.y);
	}
	else
	{
		coords = start;
		ucoords = end;
	}
	//float m_smoothPath[MAX_SMOOTH * 3];
	const int MAX_POLYS = 256*2;
	//int m_nsmoothPath = 0;
	float * m_spos = (float*)&coords;
	float * m_epos = (float*)&ucoords;
	float pExtents[3] = { 2.0f, 4.0f, 2.0f }; // size of box around start/end points to look for nav polygons
	dtPolyRef m_startRef;
	//float StartNearest[3];
	dtPolyRef m_endRef;
	//float EndNearest[3];
	dtPolyRef m_polys[65536];
	int m_npolys = 0;
	//float StraightPath[MAX_PATHVERT * 3];
	int nVertCount = 0;
	//PATHDATA * path_store = m_PathStore.get();

	// setup the filter
	dtQueryFilter m_filter;
	m_filter.setIncludeFlags(0xFFFF);
	m_filter.setExcludeFlags(0);
	m_filter.setAreaCost(SAMPLE_POLYAREA_GROUND, 1.0f);

	// find the start polygon
	status = m_navQuery->findNearestPoly((float*)&coords, pExtents, &m_filter, &m_startRef, 0);
	if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK))
		return -1; // couldn't find a polygon

				   // find the end polygon
	status = m_navQuery->findNearestPoly((float*)&ucoords, pExtents, &m_filter, &m_endRef, 0);
	if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK))
		return -2; // couldn't find a polygon

	if (m_startRef && m_endRef)
	{
		m_navQuery->findPath(m_startRef, m_endRef, m_spos, m_epos, &m_filter, m_polys, &m_npolys, MAX_POLYS);
		cout << m_npolys << endl;
		cout << m_nsmoothPath << endl;
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
			int MAX_SMOOTH = 2048*2;
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
					dtStatus status = nav_mesh->getOffMeshConnectionPolyEndPoints(prevRef, polyRef, startPos, endPos);
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
	cout << m_nsmoothPath << endl;
	last_path_points.clear();
	for (unsigned i = 0; i < m_nsmoothPath * 3; i += 3)
	{
		last_path_points.push_back(Vector3(m_smoothPath[i], m_smoothPath[i + 1], m_smoothPath[i + 2]));
	}
	return true;
}
bool GlobalNavMesh::Save()
{
	const dtNavMesh * mesh =nav_mesh;
	if (!mesh) return true;

	FILE* fp = fopen("saved.bin", "wb");
	if (!fp)
		return true;

	// Store header.
	NavMeshSetHeader header;
	header.magic = NAVMESHSET_MAGIC;
	header.version = NAVMESHSET_VERSION;
	header.numTiles = 0;
	for (int i = 0; i < mesh->getMaxTiles(); ++i)
	{
		const dtMeshTile* tile = mesh->getTile(i);
		if (!tile || !tile->header || !tile->dataSize) continue;
		header.numTiles++;
	}
	memcpy(&header.params, nav_mesh->getParams(), sizeof(dtNavMeshParams));
	fwrite(&header, sizeof(NavMeshSetHeader), 1, fp);

	// Store tiles.
	for (int i = 0; i < nav_mesh->getMaxTiles(); ++i)
	{
		const dtMeshTile* tile = mesh->getTile(i);
		if (!tile || !tile->header || !tile->dataSize) continue;

		NavMeshTileHeader tileHeader;
		tileHeader.tileRef = nav_mesh->getTileRef(tile);
		tileHeader.dataSize = tile->dataSize;
		fwrite(&tileHeader, sizeof(tileHeader), 1, fp);

		fwrite(tile->data, tile->dataSize, 1, fp);
	}

	fclose(fp);
	return false;
}

bool GlobalNavMesh::Load()
{
	FILE* fp = fopen("saved.bin", "rb");
	if (!fp) return 0;

	// Read header.
	NavMeshSetHeader header;
	size_t readLen = fread(&header, sizeof(NavMeshSetHeader), 1, fp);
	if (readLen != 1)
	{
		fclose(fp);
		return 0;
	}
	if (header.magic != NAVMESHSET_MAGIC)
	{
		fclose(fp);
		return 0;
	}
	if (header.version != NAVMESHSET_VERSION)
	{
		fclose(fp);
		return 0;
	}

	nav_mesh = dtAllocNavMesh();
	dtNavMesh* mesh = nav_mesh;// dtAllocNavMesh();
	if (!mesh)
	{
		fclose(fp);
		return 0;
	}
	dtStatus status = mesh->init(&header.params);
	if (dtStatusFailed(status))
	{
		fclose(fp);
		return 0;
	}

	// Read tiles.
	for (int i = 0; i < header.numTiles; ++i)
	{
		NavMeshTileHeader tileHeader;
		readLen = fread(&tileHeader, sizeof(tileHeader), 1, fp);
		if (readLen != 1)
			return 0;

		if (!tileHeader.tileRef || !tileHeader.dataSize)
			break;

		unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
		if (!data) break;
		memset(data, 0, tileHeader.dataSize);
		readLen = fread(data, tileHeader.dataSize, 1, fp);
		if (readLen != 1)
			return 0;

		mesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
	}

	fclose(fp);
	return true;
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
		Utils::Geometry::BoundingBox & bbox = model->GetBoundingBox();
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