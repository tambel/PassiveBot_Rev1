#include "stdafx.h"
#include <Windows.h>

unsigned short* ChunkModel::indices=ChunkModel::Init();

Chunk::Chunk(void)
{
}
Chunk::Chunk(ChunkStreamInfo info, ChunkStreamInfo obj_info, ADT * adt, Location * location, Point2D<int> block_coordinates,Point2D<int> coordinates):root_info(info),location(location),block_coordinates(block_coordinates),coordinates(coordinates)
{
	yey=false;
	result_mesh = 0;
	this->adt = adt;
	doodads = vector<Doodad>();
	wmos = vector<WMO>();
	header=MCNK();
	root_info=info;
	root_reader=root_info.reader;
	this->obj_reader = obj_info.reader;
	indices=ChunkModel::indices;
	index_count=ChunkModel::index_count;
	vertex_count=ChunkModel::vertex_count;
	//root_reader->GetStream()->seekg(root_info.start,ios::beg);
	root_reader->SetPosition(root_info.start);
	header =root_reader->Read<MCNK>();
	game_position=header.position;
	//root_reader->GetStream()->read((char*)&header,sizeof(MCNK));
	unsigned sig;
	unsigned size;
	while (root_reader->GetPosition()<=root_info.start+root_info.size-8)
	{
		sig=root_reader->ReadUInt();
		size=root_reader->ReadUInt();
		switch (sig)
		{
		case Utils::ChunkSignatures::ADTSignature::MCNKSignatures::Mcvt:
		{
			LoadMcvt();
			break;
		}
		default:
			break;
		}
	}
	obj_reader->SetPosition(obj_info.start);
	while (obj_reader->GetPosition() <= obj_info.start + obj_info.size-8)
	{
		sig = obj_reader->ReadUInt();
		size = obj_reader->ReadUInt();
		switch (sig)
		{
		case Utils::ChunkSignatures::ADTSignature::MCNKSignatures::Mcrd:
		{
			LoadMcrd(size);
			break;
		}
		case Utils::ChunkSignatures::ADTSignature::MCNKSignatures::Mcrw:
		{	
			LoadMcrw(size);
			break;
		}
		default:
			break;
		}
	}
	//InitNavigation();

	//root_reader->ReadBytes((char*)&height,

}
Chunk::~Chunk(void)
{
}
void Chunk::LoadMcvt()
{
	
	float * heights=new float[145];
	root_reader->ReadArray<float>((char*)heights,145);
	position.coords=Vector3(0,0,header.position.z);
	real_position=Vector3(Metrics::MapMidPoint - header.position.y,-(Metrics::MapMidPoint - header.position.x),header.position.z);
	/*
	float posx = Metrics::MapMidPoint - header.position.y;
	float posy = Metrics::MapMidPoint + header.position.x;
	float posz = header.position.z;
	*/
	game_position= Vector3(-game_position.y,game_position.x,game_position.z);
				
	//posx=0;
	//posy=0;
	int counter = 0;
	vertices=new Utils::Graphics::Vertex[145];
	for(int i = 0; i < 17; ++i)
	{
		for(int j = 0; j < (((i % 2) != 0) ? 8 : 9); ++j)
		{
			float height = position.coords.z + heights[counter];
			float x = position.coords.x + j * Metrics::UnitSize;
			if ((i % 2) != 0)
				x += 0.5f * Metrics::UnitSize;
			float y = position.coords.y - i * Metrics::UnitSize * 0.5f;

			vertices[counter].position =Vector3(x, y, heights[counter]);
			
			if ((i % 2) != 0)
			{

				vertices[counter].color=Graphics::Color(1.0f,1.0f,1.0f,1.0f);
				
			}
			else
			{
				vertices[counter].color=Graphics::Color(0.0f,0.0f,0.0f,0.0f);
				
				
			}
			if (i == 0 && (j == 0 || j == 1))
				{
					
					vertices[counter].color=Graphics::Color(0.0f,1.0f,1.0f,0.0f);
				}
			++counter;
		}
	}

	delete [] heights;
}
void Chunk::LoadMcrd(unsigned long size)
{
	unsigned count = size / 4;
	unique_ptr<unsigned> doodads_refs = unique_ptr<unsigned>(new unsigned[count]);
	obj_reader->ReadArray<unsigned>(doodads_refs.get(), count);
	if (coordinates.X == 3 && coordinates.X == 15)
	{
		count = count;
	}
	for (unsigned i = 0; i < count; i++)
	{
		
		MDDF mddf = adt->GetMDDFs()[doodads_refs.get()[i]];
		if (mddf.Scale != 1024)
		{
			mddf.Scale = mddf.Scale;
		}
		string filename= Configuration::GetGameDataPath()+(adt->GetDoodadsFilenames()+adt->GetDoodadsIds()[mddf.Mmid]);
		Doodad doodad = Doodad(filename, mddf.UniqueId, Position(mddf.Position, mddf.Rotation), mddf.Scale);

		float f = doodad.GetVertices()[10].position.x;
		for (unsigned long ii = 0; ii < doodad.GetVertexCount(); ii++)
		{
		//	doodad.GetVertices()[ii].position= doodad.GetVertices()[ii].position+ doodad.GetPosition().coords;
		}
		doodads.push_back(move(doodad));
	}
}
void Chunk::LoadMcrw(unsigned long size)
{
	unsigned count = size / 4;
	unique_ptr<unsigned> wmo_refs = unique_ptr<unsigned>(new unsigned[count]);
	obj_reader->ReadArray<unsigned>(wmo_refs.get(), count);
	for (unsigned i = 0; i < count; i++)
	{
		MODF modf = adt->GetMODFs()[wmo_refs.get()[i]];
		string filename = Configuration::GetGameDataPath() + (adt->GetWMOFilenames() + adt->GetWMOsIds()[modf.Mwid]);
		WMO wmo = WMO(filename, modf.UniqueId, Position(modf.Position, modf.Rotation));
		wmos.push_back(move(wmo));

	}
}
void Chunk::InitNavigation()
{
	//unsigned long vert_count=0;
	//unsigned long ind_count = 0;;
	//unique_ptr<float> vertices;
	//unique_ptr<int> indices;
	////float * nav_vertices;
	vert_count = 0;
	ind_count = 0;
	vert_count += vertex_count;
	ind_count += index_count;
	for (auto &doodad : doodads)
	{
		vert_count += doodad.GetVertexCount();
		ind_count += doodad.GetIndexCount();
	}
	for (auto &wmo : wmos)
	{
		for (auto &part : wmo.GetParts())
		{
			vert_count += part.GetVertexCount();
			ind_count += part.GetIndexCount();
		}
	}
	nav_vertices = unique_ptr<float>(new float[vert_count*3]);
	nav_indices = unique_ptr<int>(new int[ind_count]);
	unsigned long vert_offset = 0;
	terrain_bounding_box.up;
	for (unsigned i = 0; i < 145; i++)
	{
		/*vertices.get()[vert_offset] = this->vertices[i].position.x + position.coords.x;
		vertices.get()[vert_offset+1] = this->vertices[i].position.z + position.coords.z;
		vertices.get()[vert_offset+2] = this->vertices[i].position.y + position.coords.y;*/

		nav_vertices.get()[vert_offset] = this->vertices[i].position.x + real_position.x;
		nav_vertices.get()[vert_offset+1] = this->vertices[i].position.z + real_position.z;
		nav_vertices.get()[vert_offset+2] = this->vertices[i].position.y + real_position.y;
		vert_offset += 3;
	}
	rcCalcBounds(nav_vertices.get(), vertex_count, reinterpret_cast<float*>(&terrain_bounding_box.up), reinterpret_cast<float*>(&terrain_bounding_box.down));

	for (auto &doodad : doodads)
	{
		
		//doodad.Rescale(doodad.scale);
		//doodad.Rotate();
		for (unsigned long i = 0; i < doodad.GetVertexCount(); i++)
		{
			//Vector3 real= Vector3(Metrics::MapMidPoint - doodad.GetGamePosition().y, -(Metrics::MapMidPoint - doodad.GetGamePosition().x), doodad.GetGamePosition().z);

			//Vector3 vert = doodad.GetVertices()[i].position + (doodad.GetPosition().coords - real_position);// +position.coords;
			//vertices.get()[vert_offset] = vert.x + position.coords.x;
			//vertices.get()[vert_offset + 1] = vert.z +position.coords.z;
			//vertices.get()[vert_offset + 2] = vert.y + position.coords.y;

			nav_vertices.get()[vert_offset] = doodad.GetVertices()[i].position.x + doodad.GetPosition().coords.x;
			nav_vertices.get()[vert_offset + 1] = doodad.GetVertices()[i].position.z + doodad.GetPosition().coords.z;
			nav_vertices.get()[vert_offset + 2] = doodad.GetVertices()[i].position.y + doodad.GetPosition().coords.y;


			/*vertices.get()[vert_offset] = doodad.GetVertices()[i].position.x;
			vertices.get()[vert_offset + 1] = doodad.GetVertices()[i].position.z ;
			vertices.get()[vert_offset + 2] = doodad.GetVertices()[i].position.y ;*/

			vert_offset += 3;
		}
	}
	for (auto &wmo : wmos)
	{
		for (auto &part : wmo.GetParts())
		{
			for (unsigned long i = 0; i < part.GetVertexCount(); i++)
			{
				Vector3 vert = part.GetVertices()[i].position + (wmo.GetPosition().coords - real_position);

				nav_vertices.get()[vert_offset] = part.GetVertices()[i].position.x+wmo.GetPosition().coords.x;
				nav_vertices.get()[vert_offset + 1] = part.GetVertices()[i].position.z + wmo.GetPosition().coords.z;
				nav_vertices.get()[vert_offset + 2] = part.GetVertices()[i].position.y + wmo.GetPosition().coords.y;


				//vertices.get()[vert_offset] = part.GetVertices()[i].position.x;
				//vertices.get()[vert_offset + 1] = part.GetVertices()[i].position.z;
				//vertices.get()[vert_offset + 2] = part.GetVertices()[i].position.y;
				
				/*vertices.get()[vert_offset] = vert.x+position.coords.x;
				vertices.get()[vert_offset + 1] = vert.z + position.coords.z;
				vertices.get()[vert_offset + 2] = vert.y + position.coords.y;*/
				vert_offset += 3;
			}
		}
	}
	/*if (yey)
	for (unsigned long vi = 0; vi < vert_count * 3; vi += 3)
	{
		vertices.get()[vi] += 34.0f;
	}*/
	vert_offset = 0;
	unsigned long ind_offset = 0;
	for (unsigned i = 0; i < 768; i+=3)
	{
		nav_indices.get()[ind_offset] = this->indices[i + 2] + vert_offset;
		nav_indices.get()[ind_offset + 1] = this->indices[i + 1] + vert_offset;
		nav_indices.get()[ind_offset + 2] = this->indices[i] + vert_offset;
		ind_offset += 3;
	}
	vert_offset += this->vertex_count;
	for (auto &doodad : doodads)
	{
		for (unsigned long i = 0; i < doodad.GetIndexCount(); i+=3)
		{
			nav_indices.get()[ind_offset] = doodad.GetIndices()[i + 2] + vert_offset;
			nav_indices.get()[ind_offset + 1] = doodad.GetIndices()[i + 1] + vert_offset;
			nav_indices.get()[ind_offset + 2] = doodad.GetIndices()[i] + vert_offset;
			ind_offset += 3;
		}
		vert_offset += doodad.GetVertexCount();
	}
	for (auto &wmo : wmos)
	{
		for (auto &part : wmo.GetParts())
		{
			for (unsigned long i = 0; i < part.GetIndexCount(); i+=3)
			{
				nav_indices.get()[ind_offset] = part.GetIndices()[i + 2] + vert_offset;
				nav_indices.get()[ind_offset + 1] = part.GetIndices()[i + 1] + vert_offset;
				nav_indices.get()[ind_offset + 2] = part.GetIndices()[i] + vert_offset;
				ind_offset += 3;
			}
			vert_offset += part.GetVertexCount();
		}
	}
	ofstream file;
	file.open(to_string(coordinates.X)+" "+to_string(coordinates.Y)+" test.obj");
	for (unsigned vi = 0; vi < vert_count*3; vi+=3)
	{
		file << "v " << nav_vertices.get()[vi] << " " << nav_vertices.get()[vi + 1] << " " << nav_vertices.get()[vi + 2] << endl;
	}
	for (unsigned ii = 0; ii < ind_count; ii += 3)
	{
		file << "f " << nav_indices.get()[ii]+1 << " " << nav_indices.get()[ii + 1]+1 << " " << nav_indices.get()[ii + 2]+1 << endl;
	}
	file.close();
	////
	return;


	//dtNavMeshQuery * m_navQuery = 0;;
	//float bmin[3];
	//float bmax[3];
	//rcPolyMesh * result_mesh;


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


	rcCalcBounds(nav_vertices.get(), vert_count, bmin, bmax);

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
	rcMarkWalkableTriangles(m_ctx, m_cfg.walkableSlopeAngle, nav_vertices.get(), vert_count, nav_indices.get(), ntris, m_triareas);
	if (!rcRasterizeTriangles(m_ctx, nav_vertices.get(), vert_count, nav_indices.get(), m_triareas, ntris, *m_solid, m_cfg.walkableClimb))
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
		//unsigned char* navData = 0;
		//int navDataSize = 0;
		navDataSize = 0;
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
		params.tileX = 0;
		params.tileY = 1;
		
		rcVcopy(params.bmin, m_pmesh->bmin);
		rcVcopy(params.bmax, m_pmesh->bmax);
		if (yey)
		{
			params.tileX = 0;
			params.tileY = 1;
			/*m_pmesh->bmin[0] -= 33;
			m_pmesh->bmin[1] -= 33;
			m_pmesh->bmin[2] -= 33;
			m_pmesh->bmax[0] -= 33;
			m_pmesh->bmax[1] -= 33;
			m_pmesh->bmax[2] -= 33;
			rcVcopy(params.bmin, m_pmesh->bmin);
			rcVcopy(params.bmax, m_pmesh->bmax);*/
		}
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
		m_navQuery = new dtNavMeshQuery();
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
bool Chunk::operator==(const Chunk & right)
{
	return this->location->id==right.location->id && this->block_coordinates==right.block_coordinates && this->coordinates==right.coordinates;
}
void Chunk::SearchForObjects()
{

}