#include "stdafx.h"
#include "Navigation.h"




WowBuildContext::WowBuildContext() :rcContext()
{

}
WowBuildContext::~WowBuildContext()
{

}
void WowBuildContext::doLog(const rcLogCategory log, const char * mes, const int len)
{
	cout << mes << endl;
}


void Navigation::SetArea(SquareArea & area)
{

	Chunk *** chunks = area.GetChunks();
	int area_size = area.GetRadius() * 2 + 1;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			vert_count += chunks[i][j]->GetVertexCount();
			ind_count += chunks[i][j]->GetIndexCount();
		}
	}
	for (auto doodad : *area.GetActiveDoodas())
	{
		vert_count += doodad->GetVertexCount();
		ind_count += doodad->GetIndexCount();

	}
	for (auto wmo : area.GetActiveWMOs())
	{
		for (auto &part : wmo->GetParts())
		{
			vert_count += part.GetVertexCount();
			ind_count += part.GetIndexCount();
		}
	}

	vertices = new float[vert_count * 3];
	indices = new int[ind_count];
	unsigned long vert_offset = 0;
	Vector3 vect;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{

			for (int vi = 0; vi < chunks[i][j]->GetVertexCount(); vi++)
			{
				vect = chunks[i][j]->GetRealPosition() - area.GetBoundingBox().up + chunks[i][j]->GetVertices()[vi].position;
				vertices[vert_offset] = vect.x;
				vertices[vert_offset + 1] = vect.z;
				vertices[vert_offset + 2] = vect.y;
				vert_offset += 3;
			}

		}
	}
	for (auto doodad : *area.GetActiveDoodas())
	{
		for (int vi = 0; vi < doodad->GetVertexCount(); vi++)
		{
			vect = doodad->GetPosition().coords - area.GetBoundingBox().up + doodad->GetVertices()[vi].position;
			vertices[vert_offset] = vect.x;
			vertices[vert_offset + 1] = vect.z;
			vertices[vert_offset + 2] = vect.y;
			vert_offset += 3;
		}
	}
	for (auto wmo : area.GetActiveWMOs())
	{
		for (auto &part : wmo->GetParts())
		{

			for (int vi = 0; vi < part.GetVertexCount(); vi++)
			{
				vect = wmo->GetPosition().coords - area.GetBoundingBox().up + part.GetVertices()[vi].position;
				vertices[vert_offset] = vect.x;
				vertices[vert_offset + 1] = vect.z;
				vertices[vert_offset + 2] = vect.y;
				vert_offset += 3;

			}
		}
	}
	unsigned long ind_offset = 0;
	vert_offset = 0;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			for (unsigned long ii = 0; ii < chunks[i][j]->GetIndexCount(); ii += 3)
			{
				indices[ind_offset] = chunks[i][j]->GetIndices()[ii + 2]  + vert_offset;
				indices[ind_offset + 1] = chunks[i][j]->GetIndices()[ii + 1]  + vert_offset;
				indices[ind_offset + 2] = chunks[i][j]->GetIndices()[ii]  + vert_offset;
				ind_offset += 3;
			}
			vert_offset += chunks[i][j]->GetVertexCount();
		}
	}
	for (auto doodad : *area.GetActiveDoodas())
	{
		for (unsigned long ii = 0; ii < doodad->GetIndexCount(); ii += 3)
		{
			indices[ind_offset] = doodad->GetIndices()[ii + 2]  + vert_offset;
			indices[ind_offset + 1] = doodad->GetIndices()[ii + 1]  + vert_offset;
			indices[ind_offset + 2] = doodad->GetIndices()[ii]  + vert_offset;
			ind_offset += 3;
		}
		vert_offset += doodad->GetVertexCount();
	}
	for (auto wmo : area.GetActiveWMOs())
	{
		for (auto &part : wmo->GetParts())
		{
			for (unsigned long ii = 0; ii < part.GetIndexCount(); ii += 3)
			{
				indices[ind_offset] = part.GetIndices()[ii + 2]  + vert_offset;
				indices[ind_offset + 1] = part.GetIndices()[ii + 1]  + vert_offset;
				indices[ind_offset + 2] = part.GetIndices()[ii]  + vert_offset;
				ind_offset += 3;
			}
			vert_offset += part.GetVertexCount();
		}
	}
	ToFile();
	BuildNavMesh();
	delete[] vertices;
	delete[] indices;
	vertices = 0;
	indices = 0;
	vert_count = 0;
	ind_count = 0;
}

void Navigation::BuildNavMesh()
{




	//delete result_mesh;
	rcConfig m_cfg;
	rcCompactHeightfield * m_chf;
	rcContourSet * m_cset;
	rcPolyMesh * m_pmesh;
	rcPolyMeshDetail * m_dmesh;
	//rcContext * m_ctx = new rcContext();
	WowBuildContext * m_ctx = new WowBuildContext();
	memset(&m_cfg, 0, sizeof(m_cfg));
	rcHeightfield* m_solid;
	unsigned char *m_triareas;
	unsigned long ntris = ind_count / 3;
	float m_cellSize = 0.300000012;
	float m_cellHeight = 0.200000003;
	float m_agentHeight = 2.0;
	float m_agentRadius = 1.0f; //0.600000024;
	float m_agentMaxClimb = 0.899999976;
	float m_agentMaxSlope = 45.0000000;
	float m_regionMinSize = 8.00000000;
	float m_regionMergeSize = 20.0000000;
	float m_edgeMaxLen = 12.0000000;
	float m_edgeMaxError = 1.29999995;
	float m_vertsPerPoly = 6.00000000;
	float m_detailSampleDist = 6.00000000;
	float m_detailSampleMaxError = 1.00000000;
	int m_partitionType = 0;
	if (m_navQuery)
	{
		dtFreeNavMeshQuery(m_navQuery);
		m_navQuery = 0;
	}

	m_cfg.cs = m_cellSize;
	m_cfg.ch = m_cellHeight;
	m_cfg.walkableSlopeAngle = m_agentMaxSlope;
	m_cfg.walkableHeight = (int)ceilf(m_agentHeight / m_cfg.ch);
	m_cfg.walkableClimb = (int)floorf(m_agentMaxClimb / m_cfg.ch);
	m_cfg.walkableRadius = (int)ceilf(m_agentRadius / m_cfg.cs);
	m_cfg.maxEdgeLen = (int)(m_edgeMaxLen / m_cellSize);
	m_cfg.maxSimplificationError = m_edgeMaxError;
	m_cfg.minRegionArea = (int)rcSqr(m_regionMinSize);		// Note: area = size*size
	m_cfg.mergeRegionArea = (int)rcSqr(m_regionMergeSize);	// Note: area = size*size
	m_cfg.maxVertsPerPoly = (int)m_vertsPerPoly;
	m_cfg.detailSampleDist = m_detailSampleDist < 0.9f ? 0 : m_cellSize * m_detailSampleDist;
	m_cfg.detailSampleMaxError = m_cellHeight * m_detailSampleMaxError;



	rcCalcBounds(vertices, vert_count, bmin, bmax);
	
	rcVcopy(m_cfg.bmin, bmin);
	rcVcopy(m_cfg.bmax, bmax);
	rcCalcGridSize(m_cfg.bmin, m_cfg.bmax, m_cfg.cs, &m_cfg.width, &m_cfg.height);

	m_ctx->resetTimers();

	// Start the build process.	
	m_ctx->startTimer(RC_TIMER_TOTAL);

	m_solid = rcAllocHeightfield();
	if (!m_solid)
	{
		throw exception("buildNavigation: Out of memory 'solid'");
	}
	if (!rcCreateHeightfield(m_ctx, *m_solid, m_cfg.width, m_cfg.height, m_cfg.bmin, m_cfg.bmax, m_cfg.cs, m_cfg.ch))
	{
		throw exception("buildNavigation: Could not create solid heightfield.");
	}
	try {
		m_triareas = new unsigned char[ntris];
	}
	catch (std::exception e)
	{

	}
	if (!m_triareas)
	{
		throw exception("buildNavigation: Out of memory 'm_triareas' (%d).", ntris);
	}

	memset(m_triareas, 0, ntris*sizeof(unsigned char));
	rcMarkWalkableTriangles(m_ctx, m_cfg.walkableSlopeAngle, vertices, vert_count, indices, ntris, m_triareas);
	if (!rcRasterizeTriangles(m_ctx, vertices, vert_count, indices, m_triareas, ntris, *m_solid, m_cfg.walkableClimb))
	{
		throw exception("buildNavigation: Could not rasterize triangles.");
	}
	delete[] m_triareas;
	m_triareas = 0;


	rcFilterLowHangingWalkableObstacles(m_ctx, m_cfg.walkableClimb, *m_solid);
	rcFilterLedgeSpans(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid);
	rcFilterWalkableLowHeightSpans(m_ctx, m_cfg.walkableHeight, *m_solid);


	m_chf = rcAllocCompactHeightfield();
	if (!m_chf)
	{
		throw exception("buildNavigation: Out of memory 'chf'.");
	}
	if (!rcBuildCompactHeightfield(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid, *m_chf))
	{
		throw exception("buildNavigation: Could not build compact data.");
	}
	rcFreeHeightField(m_solid);
	m_solid = 0;

	if (!rcErodeWalkableArea(m_ctx, m_cfg.walkableRadius, *m_chf))
	{
		throw exception("buildNavigation: Could not erode.");
	}

	if (m_partitionType == 0)
	{
		// Prepare for region partitioning, by calculating distance field along the walkable surface.
		if (!rcBuildDistanceField(m_ctx, *m_chf))
		{
			throw exception("buildNavigation: Could not build distance field.");
		}

		// Partition the walkable surface into simple regions without holes.
		if (!rcBuildRegions(m_ctx, *m_chf, 0, m_cfg.minRegionArea, m_cfg.mergeRegionArea))
		{
			throw exception("buildNavigation: Could not build watershed regions.");
		}
	}

	m_cset = rcAllocContourSet();
	if (!m_cset)
	{
		throw exception("buildNavigation: Out of memory 'cset'.");

	}
	if (!rcBuildContours(m_ctx, *m_chf, m_cfg.maxSimplificationError, m_cfg.maxEdgeLen, *m_cset))
	{
		throw exception("buildNavigation: Could not create contours.");
	}


	m_pmesh = rcAllocPolyMesh();
	if (!m_pmesh)
	{
		throw exception("buildNavigation: Out of memory 'pmesh'.");
	}
	if (!rcBuildPolyMesh(m_ctx, *m_cset, m_cfg.maxVertsPerPoly, *m_pmesh))
	{
		throw exception("buildNavigation: Could not triangulate contours.");
	}


	m_dmesh = rcAllocPolyMeshDetail();
	if (!m_dmesh)
	{
		throw exception("buildNavigation: Out of memory 'pmdtl'.");
	}

	if (!rcBuildPolyMeshDetail(m_ctx, *m_pmesh, *m_chf, m_cfg.detailSampleDist, m_cfg.detailSampleMaxError, *m_dmesh))
	{
		throw exception("buildNavigation: Could not build detail mesh.");
	}

	rcFreeCompactHeightfield(m_chf);
	m_chf = 0;
	rcFreeContourSet(m_cset);
	m_cset = 0;

	if (m_cfg.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
	{
		unsigned char* navData = 0;
		int navDataSize = 0;

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
		static const int MAX_OFFMESH_CONNECTIONS = 256;
		float m_offMeshConVerts[MAX_OFFMESH_CONNECTIONS * 3 * 2];
		float m_offMeshConRads[MAX_OFFMESH_CONNECTIONS];
		unsigned char m_offMeshConDirs[MAX_OFFMESH_CONNECTIONS];
		unsigned char m_offMeshConAreas[MAX_OFFMESH_CONNECTIONS];
		unsigned short m_offMeshConFlags[MAX_OFFMESH_CONNECTIONS];
		unsigned int m_offMeshConId[MAX_OFFMESH_CONNECTIONS];
		int m_offMeshConCount;
		memset(&params, 0, sizeof(params));
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
		params.offMeshConVerts = m_offMeshConVerts;
		params.offMeshConRad = m_offMeshConRads;
		params.offMeshConDir = m_offMeshConDirs;
		params.offMeshConAreas = m_offMeshConAreas;
		params.offMeshConFlags = m_offMeshConFlags;
		params.offMeshConUserID = m_offMeshConId;
		params.offMeshConCount = 0;// m_offMeshConCount;
		params.walkableHeight = m_agentHeight;
		params.walkableRadius = m_agentRadius;
		params.walkableClimb = m_agentMaxClimb;
		rcVcopy(params.bmin, m_pmesh->bmin);
		rcVcopy(params.bmax, m_pmesh->bmax);
		params.cs = m_cfg.cs;
		params.ch = m_cfg.ch;
		params.buildBvTree = true;

		if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
		{
			m_ctx->log(RC_LOG_ERROR, "Could not build Detour navmesh.");
		}

		dtNavMesh * m_navMesh = dtAllocNavMesh();
		if (!m_navMesh)
		{
			dtFree(navData);
			m_ctx->log(RC_LOG_ERROR, "Could not create Detour navmesh");
		}

		dtStatus status;

		status = m_navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
		if (dtStatusFailed(status))
		{
			dtFree(navData);
			m_ctx->log(RC_LOG_ERROR, "Could not init Detour navmesh");
		}
		m_navQuery=new dtNavMeshQuery();
		status = m_navQuery->init(m_navMesh, 2048);
		if (dtStatusFailed(status))
		{
			m_ctx->log(RC_LOG_ERROR, "Could not init Detour navmesh query");
		}
	}

	m_ctx->stopTimer(RC_TIMER_TOTAL);
	if (m_ctx) delete m_ctx;
	result_mesh = m_pmesh;


}

int Navigation::FindPath(Vector3 & pStartPos, Vector3 & pEndPos, int nPathSlot, int nTarget)
{
	dtStatus status;
	float pExtents[3] = { 32.0f, 32.0f, 32.0f }; // size of box around start/end points to look for nav polygons
	dtPolyRef StartPoly;
	float StartNearest[3];
	dtPolyRef EndPoly;
	float EndNearest[3];
	dtPolyRef PolyPath[MAX_PATHPOLY];
	int nPathCount = 0;
	float StraightPath[MAX_PATHVERT * 3];
	int nVertCount = 0;


	// setup the filter
	dtQueryFilter Filter;
	Filter.setIncludeFlags(0xFFFF);
	Filter.setExcludeFlags(0);
	Filter.setAreaCost(SAMPLE_POLYAREA_GROUND, 1.0f);

	// find the start polygon
	status = m_navQuery->findNearestPoly((float*)&pStartPos, pExtents, &Filter, &StartPoly, StartNearest);
	if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK)) return -1; // couldn't find a polygon

																		  // find the end polygon
	status = m_navQuery->findNearestPoly((float*)&pEndPos, pExtents, &Filter, &EndPoly, EndNearest);
	if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK)) return -2; // couldn't find a polygon

	status = m_navQuery->findPath(StartPoly, EndPoly, StartNearest, EndNearest, &Filter, PolyPath, &nPathCount, MAX_PATHPOLY);
	if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK)) return -3; // couldn't create a path
	if (nPathCount == 0) return -4; // couldn't find a path

	status = m_navQuery->findStraightPath(StartNearest, EndNearest, PolyPath, nPathCount, StraightPath, NULL, NULL, &nVertCount, MAX_PATHVERT+100);
	if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK)) return -5; // couldn't create a path
	if (nVertCount == 0) return -6; // couldn't find a path

									// At this point we have our path.  Copy it to the path store
	int nIndex = 0;
	for (int nVert = 0; nVert<nVertCount; nVert++)
	{
		m_PathStore[nPathSlot].PosX[nVert] = StraightPath[nIndex++];
		m_PathStore[nPathSlot].PosY[nVert] = StraightPath[nIndex++];
		m_PathStore[nPathSlot].PosZ[nVert] = StraightPath[nIndex++];

		//sprintf(m_chBug, "Path Vert %i, %f %f %f", nVert, m_PathStore[nPathSlot].PosX[nVert], m_PathStore[nPathSlot].PosY[nVert], m_PathStore[nPathSlot].PosZ[nVert]) ;
		//m_pLog->logMessage(m_chBug);
	}
	m_PathStore[nPathSlot].MaxVertex = nVertCount;
	m_PathStore[nPathSlot].Target = nTarget;
	return nVertCount;
}


Navigation::Navigation()
{
	m_navQuery = 0;
	result_mesh = 0;
	vert_count = 0;
	ind_count = 0;
	vertices=0;
	indices=0;
}

Navigation::~Navigation()
{
	dtFreeNavMeshQuery(m_navQuery);
	m_navQuery = 0;
	delete result_mesh;
	delete[] vertices;
	delete[] indices;

}

Navigation & Navigation::operator=(Navigation && right)
{
	m_navQuery = right.m_navQuery;
	right.m_navQuery = nullptr;
	result_mesh = right.result_mesh;
	right.result_mesh = nullptr;
	return *this;
}



Model<unsigned>* Navigation::GetModel()
{

	const int nvp = result_mesh->nvp;
	const float cs = result_mesh->cs;
	const float ch = result_mesh->ch;
	const float* orig = result_mesh->bmin;

	int m_flDataX = result_mesh->npolys;
	int m_flDataY = result_mesh->nverts;

	// create scenenodes


	int nIndex = 0;
	int m_nAreaCount = result_mesh->npolys;
	int n = 0;

	float * vert = new float[result_mesh->nverts * 3];
	unsigned *ind = new unsigned[result_mesh->maxpolys * 3];
	unsigned vc = 0;
	unsigned ic = 0;
	if (m_nAreaCount)
	{

		// start defining the manualObject
		for (int i = 0; i < result_mesh->npolys; ++i) // go through all polygons
			if (result_mesh->areas[i] == 0)
			{
				const unsigned short* p = &result_mesh->polys[i*nvp * 2];

				unsigned short vi[3];
				for (int j = 2; j < nvp; ++j) // go through all verts in the polygon
				{
					if (p[j] == RC_MESH_NULL_IDX) break;
					vi[0] = p[0];
					vi[1] = p[j - 1];
					vi[2] = p[j];
					for (int k = 0; k < 3; ++k) // create a 3-vert triangle for each 3 verts in the polygon.
					{
						const unsigned short* v = &result_mesh->verts[vi[k] * 3];
						const float x = orig[0] + v[0] * cs;
						const float y = orig[1] + (v[1] + 1)*ch;
						const float z = orig[2] + v[2] * cs;


						vert[vc] = x;
						vert[vc + 1] = y;
						vert[vc + 2] = z;
						vc += 3;
						cout << n << endl;
						n++;

					}
					//m_pRecastMOWalk->triangle(nIndex, nIndex + 1, nIndex + 2);
					//m_pRecastMOWalk->index(nIndex);
					//m_pRecastMOWalk->index(nIndex + 1);
					//m_pRecastMOWalk->index(nIndex + 2);

					ind[ic] = nIndex + 1;
					ind[ic + 1] = nIndex + 2;
					ind[ic + 2] = nIndex + 3;
					ic += 3;
					nIndex += 3;
				}
			}
		//m_pRecastMOWalk->end();

		return nullptr;
	}
}

void Navigation::ToFile()
{
	ofstream file;
	file.open("area_mesh.obj");
	for (unsigned long i= 0; i < vert_count*3; i += 3)
	{
		file << "v " << vertices[i] << " " << vertices[i + 1] << " " << vertices[i + 2] << endl;
	}
	for (unsigned long i = 0; i < ind_count; i += 3)
	{
		file << "f " << indices[i]+1 << " " << indices[i + 1]+1 << " " << indices[i + 2]+1 << endl;
	}
	file.close();
}

