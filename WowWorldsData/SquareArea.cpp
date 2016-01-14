#include "stdafx.h"
#include <algorithm>
SquareArea::SquareArea()
{
}
SquareArea::SquareArea(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius) :location(location), block_coordinates(block_coordinates), coordinates(coordinates), radius(radius)
{
	polys = vector<rcPolyMesh*>();
	m_navMesh = 0;
	busy = false;
	navigation = move(Navigation());
	area_size = radius * 2 + 1;
	chunks = new Chunk**[area_size];
	doodads = vector<Doodad*>();
	to_update = true;
	old_doodads = vector<Doodad*>();
	active_doodads = vector<Doodad*>();
	wmos = vector<WMO*>();
	old_wmos = vector<WMO*>();
	active_wmos = vector<WMO*>();
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
	InitObjects();
	InitActiveObjects();
	InitBoundingBox();
	//navigation.SetArea(*this);
}


SquareArea::~SquareArea(void)
{

}

SquareArea & SquareArea::operator=(SquareArea && right)
{
	navigation = move(right.navigation);
	wmos = move(right.wmos);
	chunks = right.chunks;
	radius = right.radius;
	doodads = move(right.doodads);
	old_wmos = move(right.old_wmos);
	old_doodads = move(right.old_doodads);
	active_wmos = move(right.active_wmos);
	active_doodads = move(right.active_doodads);
	area_size = right.area_size;
	location = right.location;
	block_coordinates = right.block_coordinates;
	coordinates = right.coordinates;
	bounding_box = right.bounding_box;
	to_update = right.to_update;
	busy = right.busy;
	m_PathStore = right.m_PathStore;
	right.m_PathStore = 0;
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
			//if (i == radius  && (j == radius|| j==radius+1))
			{
				//if (j == radius + 1)
				{
					chunks[i][j]->yey = true;
				}
				chunks[i][j]->InitNavigation();
				
			}
			//if (i==0 && j==0)
			//for (int vi = 0; vi < chunks[i][j]->GetVertexCount(); vi++)
			//{
			//	chunks[i][j]->GetVertices()[vi].color = Utils::Graphics::Color(0, 0,color,1);
			//	//chunks[i][j]->GetVertices()[vi].color.a = 0;
			//	
			//}
			//color += 4;
			
		}

	}
	//DeleteDuplicates();
	InitBoundingBox();
	InitNavigationR();
//	TestNav();


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
				InitObjects();
			InitActiveObjects();
			InitBoundingBox();
			//navigation.SetArea(*this);
			to_update = true;
			busy = false;
		}

	}

}
void SquareArea::InitObjects()
{

	InitDoodads();
	InitWMOs();

}
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
void SquareArea::InitActiveObjects()
{

	
	active_doodads.clear();
	for (auto doodad : doodads)
	{
		//if (bounding_box.IsInside2D(doodad->GetPosition().coords))
		{
			active_doodads.push_back(doodad);
		}
	}
	
	active_wmos.clear();
	for (auto &wmo : wmos)
	{
		//if (bounding_box.IsInside2D(wmo->GetPosition().coords))
		{
			active_wmos.push_back(wmo);
		}
	}
}

void SquareArea::ToMesh()
{

	unsigned long vert_offset = 0;
	unsigned long ind_offset = 0;
	ofstream myfile;
	myfile.open("example.obj");
	Vector3 vect;
	vector<unsigned long> vert_offsets = vector<unsigned long>();
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			vert_offset = 0;
			for (int vi = 0; vi < chunks[i][j]->GetVertexCount(); vi++)
			{
				vect = chunks[i][j]->GetRealPosition() - this->bounding_box.up + chunks[i][j]->GetVertices()[vi].position;
				myfile << "v " << vect.x << " " << vect.z << " " << vect.y << endl;
				vert_offset++;

			}
			vert_offsets.push_back(chunks[i][j]->GetVertexCount());

		}
	}
	for (auto doodad : active_doodads)
	{
		for (int vi = 0; vi < doodad->GetVertexCount(); vi++)
		{
			vect = doodad->GetPosition().coords - this->bounding_box.up + doodad->GetVertices()[vi].position;
			myfile << "v " << vect.x << " " << vect.z << " " << vect.y << endl;
			//vert_offset++;

		}
	}
	for (auto wmo : active_wmos)
	{
		for (auto &part : wmo->GetParts())
		{

			for (int vi = 0; vi < part.GetVertexCount(); vi++)
			{
				vect = wmo->GetPosition().coords - this->bounding_box.up + part.GetVertices()[vi].position;
				myfile << "v " << vect.x << " " << vect.z << " " << vect.y << endl;
				//vert_offset++;

			}
		}
	}
	vert_offset = 0;
	ind_offset = 0;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			for (unsigned long ii = 0; ii < chunks[i][j]->GetIndexCount(); ii += 3)
			{
				myfile << "f " << chunks[i][j]->GetIndices()[ii+2] + vert_offset + 1 << " " << chunks[i][j]->GetIndices()[ii + 1] + vert_offset + 1 << " " << chunks[i][j]->GetIndices()[ii] + vert_offset + 1 << endl;
			}
			vert_offset += chunks[i][j]->GetVertexCount();
		}
	}
	
	for (auto doodad : active_doodads)
	{
		for (unsigned long ii = 0; ii < doodad->GetIndexCount(); ii+=3)
		{
			myfile << "f " << doodad->GetIndices()[ii+2] + vert_offset + 1 << " " << doodad->GetIndices()[ii + 1] + vert_offset + 1 << " " << doodad->GetIndices()[ii] + vert_offset + 1 << endl;
		}
		vert_offset += doodad->GetVertexCount();
	}
	for (auto wmo : active_wmos)
	{
		for (auto &part : wmo->GetParts())
		{
			for (unsigned long ii = 0; ii < part.GetIndexCount(); ii += 3)
			{
				myfile << "f " << part.GetIndices()[ii+2] + vert_offset + 1 << " " << part.GetIndices()[ii + 1] + vert_offset + 1 << " " << part.GetIndices()[ii] + vert_offset + 1 << endl;
			}
			vert_offset += part.GetVertexCount();
		}
	}

	myfile.close();

}

void SquareArea::InitBoundingBox()
{
	bounding_box.up.x = 99999.0f;
	bounding_box.up.y = 99999.0f;
	bounding_box.down.x = -99999.0f;
	bounding_box.down.y = -99999.0f;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			if (!chunks[i][j]) continue;
			for (unsigned long vi = 0; vi < chunks[i][j]->GetVertexCount();vi++)
			{
				if (chunks[i][j]->GetVertices()[vi].position.x + chunks[i][j]->GetRealPosition().x< bounding_box.up.x)
					bounding_box.up.x = chunks[i][j]->GetVertices()[vi].position.x + chunks[i][j]->GetRealPosition().x;
				if (chunks[i][j]->GetVertices()[vi].position.y + chunks[i][j]->GetRealPosition().y < bounding_box.up.y)
					bounding_box.up.y = chunks[i][j]->GetVertices()[vi].position.y + chunks[i][j]->GetRealPosition().y;
				if (chunks[i][j]->GetVertices()[vi].position.x + chunks[i][j]->GetRealPosition().x > bounding_box.down.x)
					bounding_box.down.x = chunks[i][j]->GetVertices()[vi].position.x + chunks[i][j]->GetRealPosition().x;
				if (chunks[i][j]->GetVertices()[vi].position.y + chunks[i][j]->GetRealPosition().y> bounding_box.down.y)
					bounding_box.down.y = chunks[i][j]->GetVertices()[vi].position.y + chunks[i][j]->GetRealPosition().y;
			}

		}
	}
	for (auto doodad : active_doodads)
	{
		for (unsigned long vi = 0; vi < doodad->GetVertexCount(); vi++)
		{
			if (doodad->GetVertices()[vi].position.x+doodad->GetPosition().coords.x < bounding_box.up.x)
				bounding_box.up.x = doodad->GetVertices()[vi].position.x+doodad->GetPosition().coords.x;
			if (doodad->GetVertices()[vi].position.y+ doodad->GetPosition().coords.y < bounding_box.up.y)
				bounding_box.up.y = doodad->GetVertices()[vi].position.y + doodad->GetPosition().coords.y;
			if (doodad->GetVertices()[vi].position.x + doodad->GetPosition().coords.x > bounding_box.down.x)
				bounding_box.down.x = doodad->GetVertices()[vi].position.x + doodad->GetPosition().coords.x;
			if (doodad->GetVertices()[vi].position.y + doodad->GetPosition().coords.y> bounding_box.down.y)
				bounding_box.down.y = doodad->GetVertices()[vi].position.y + doodad->GetPosition().coords.y;
		}
	}
	for (auto wmo : active_wmos)
	{
		for (auto &part:wmo->GetParts())
			for (unsigned long vi = 0; vi < part.GetVertexCount(); vi++)
			{
				if (part.GetVertices()[vi].position.x + wmo->GetPosition().coords.x< bounding_box.up.x)
					bounding_box.up.x = part.GetVertices()[vi].position.x + wmo->GetPosition().coords.x;
				if (part.GetVertices()[vi].position.y + wmo->GetPosition().coords.y< bounding_box.up.y)
					bounding_box.up.y = part.GetVertices()[vi].position.y + wmo->GetPosition().coords.y;
				if (part.GetVertices()[vi].position.x + wmo->GetPosition().coords.x> bounding_box.down.x)
					bounding_box.down.x = part.GetVertices()[vi].position.x + wmo->GetPosition().coords.x;
				if (part.GetVertices()[vi].position.y + wmo->GetPosition().coords.y> bounding_box.down.y)
					bounding_box.down.y = part.GetVertices()[vi].position.y + wmo->GetPosition().coords.y;
			}
	}

}

void SquareArea::TestNav()
{
	


	dtNavMeshQuery* m_navQuery = new dtNavMeshQuery();
	WowBuildContext *m_ctx = new WowBuildContext();
	float m_tileSize=32.0;
	float m_cellSize=30.0;
	int m_maxTiles=128;
	int m_maxPolysPerTile=32768;

	dtNavMesh * m_navMesh = dtAllocNavMesh();
	if (!m_navMesh)
	{
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not allocate navmesh.");
		return;
	}
	float b[3] = { 0,0,0 };
	dtNavMeshParams params;
	//rcVcopy(params.orig, m_geom->getMeshBoundsMin());
	rcVcopy(params.orig, b);
	params.tileWidth = m_tileSize*m_cellSize;
	params.tileHeight = m_tileSize*m_cellSize;
	params.maxTiles = m_maxTiles;
	params.maxPolys = m_maxPolysPerTile;

	dtStatus status;

	status = m_navMesh->init(&params);
	if (dtStatusFailed(status))
	{
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init navmesh.");
		return;
	}

	status = m_navQuery->init(m_navMesh, 2048);
	if (dtStatusFailed(status))
	{
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init Detour navmesh query");
		return;
	}
	/////////////////////////////////
	status =m_navMesh->addTile(chunks[3][3]->navData, chunks[3][3]->navDataSize, DT_TILE_FREE_DATA,0,0);
	if (dtStatusFailed(status))
	{
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init Detour navmesh query");
		return;
	}

	status = m_navMesh->addTile(chunks[3][4]->navData, chunks[3][4]->navDataSize, DT_TILE_FREE_DATA, 0, 0);
	if (dtStatusFailed(status))
	{
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init Detour navmesh query");
		return;
	}
	status = m_navQuery->init(m_navMesh, 2048);
	if (dtStatusFailed(status))
	{
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init Detour navmesh query");
		return;
	}
	float pExtents[3] = { 32.0f, 32.0f, 32.0f }; // size of box around start/end points to look for nav polygons
	dtPolyRef StartPoly;
	float StartNearest[3];
	dtPolyRef EndPoly;
	float EndNearest[3];
	dtPolyRef PolyPath[MAX_PATHPOLY];
	int nPathCount = 0;
	float StraightPath[MAX_PATHVERT * 3];
	int nVertCount = 0;

	dtQueryFilter Filter;
	Filter.setIncludeFlags(0xFFFF);
	Filter.setExcludeFlags(0);
	Filter.setAreaCost(SAMPLE_POLYAREA_GROUND, 1.0f);

	float start[3] = { 2.32590485,-0.580303431,-31.7947750 };
	float end[3] = { 12.58681488,0.148092791,-23.8900890 };

	status = m_navQuery->findNearestPoly(start, pExtents, &Filter, &StartPoly, StartNearest);
	if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK)) return ; // couldn't find a polygon

																		  // find the end polygon
	status = m_navQuery->findNearestPoly(end, pExtents, &Filter, &EndPoly, EndNearest);
	if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK)) return; // couldn't find a polygon


	status = m_navQuery->findPath(StartPoly, EndPoly, StartNearest, EndNearest, &Filter, PolyPath, &nPathCount, MAX_PATHPOLY);
	if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK)) return ; // couldn't create a path
	if (nPathCount == 0) return ; // couldn't find a path

	status = m_navQuery->findStraightPath(StartNearest, EndNearest, PolyPath, nPathCount, StraightPath, NULL, NULL, &nVertCount, MAX_PATHVERT );
	if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK)) return ; // couldn't create a path
	if (nVertCount == 0) return; // couldn't find a path
	int nIndex = 0;
	m_PathStore = new PATHDATA;
	for (int nVert = 0; nVert<nVertCount; nVert++)
	{
		m_PathStore[0].PosX[nVert] = StraightPath[nIndex++]+chunks[3][3]->GetRealPosition().x;
		m_PathStore[0].PosY[nVert] = StraightPath[nIndex++]+chunks[3][3]->GetRealPosition().z;
		m_PathStore[0].PosZ[nVert] = StraightPath[nIndex++]+chunks[3][3]->GetRealPosition().y;

		//sprintf(m_chBug, "Path Vert %i, %f %f %f", nVert, m_PathStore[nPathSlot].PosX[nVert], m_PathStore[nPathSlot].PosY[nVert], m_PathStore[nPathSlot].PosZ[nVert]) ;
		//m_pLog->logMessage(m_chBug);
	}
	m_PathStore[0].MaxVertex = nVertCount;
	//m_PathStore[0].Target = nTarget;



	/*for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			if (chunks[i][j])
			{
				if (chunks[i][j]->result_mesh)
				{

				}
			}
		}
	}*/
}

void SquareArea::InitNavigation()
{
	unique_ptr<float> vertices;
	unique_ptr<int> indices;
	unsigned index_count = 0;
	unsigned vertex_count = 0;
	float x = 0;
	float y = 0;
	Chunk *chunk;
	/*for (int i = 0; i < area_size; i++)
	{
		y = 0;
		for (int j = 0; j < area_size; j++)
		{
			chunk = chunks[i][j];
			if (!chunk)
			{
				continue;
			}
			chunk->SetPosition(Vector3(y, -x, chunk->GetPosition().coords.z));
			chunk->InitNavigation();
			y += Utils::Metrics::ChunkSize;
		}
		x += Utils::Metrics::ChunkSize;
	}*/
	////////////////////////////
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			chunk = chunks[i][j];
			if (!chunk)
			{
				continue;
			}
			vertex_count += chunk->GetVertexCount();
			index_count += chunk->GetIndexCount();
			for (auto &doodad : chunk->GetDoodads())
			{
				if (doodad.IsSkipped())
					continue;
				vertex_count += doodad.GetVertexCount();
				index_count += doodad.GetIndexCount();
			}
			for (auto &wmo : chunk->GetWMOs())
			{
				if (wmo.IsSkipped())
					continue;
				for (auto &part : wmo.GetParts())
				{
					vertex_count += part.GetVertexCount();
					index_count += part.GetIndexCount();
				}
			}
		}
	}
	vertices = unique_ptr<float>(new float[vertex_count * 3]);
	indices = unique_ptr<int>(new int[index_count]);
	float * tvertices = vertices.get();
	int * tindices = indices.get();
	unsigned vert_offset=0;
	unsigned ind_count = 0;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			chunk = chunks[i][j];
			if (!chunk)
			{
				continue;
			}
			for (unsigned vi = 0; vi < 145; vi++)
			{
				tvertices[vert_offset] = chunk->GetVertices()[vi].position.x;
				tvertices[vert_offset+1] = chunk->GetVertices()[vi].position.z;
				tvertices[vert_offset+2] = chunk->GetVertices()[vi].position.y;
				vert_offset += 3;
			}
			for (auto &doodad : chunk->GetDoodads())
			{
				if (doodad.IsSkipped())
					continue;
				for (unsigned vi = 0; vi < doodad.GetVertexCount(); vi++)
				{
					tvertices[vert_offset] = doodad.GetVertices()[vi].position.x;
					tvertices[vert_offset + 1] = doodad.GetVertices()[vi].position.z;
					tvertices[vert_offset + 2] = doodad.GetVertices()[vi].position.y;
					vert_offset += 3;
				}
			}
			for (auto &wmo : chunk->GetWMOs())
			{
				if (wmo.IsSkipped())
					continue;
				for (auto &part : wmo.GetParts())
				{
					for (unsigned vi = 0; vi < part.GetVertexCount(); vi++)
					{
						tvertices[vert_offset] = part.GetVertices()[vi].position.x;
						tvertices[vert_offset + 1] = part.GetVertices()[vi].position.z;
						tvertices[vert_offset + 2] = part.GetVertices()[vi].position.y ;
						vert_offset += 3;
					}
				}
			}
		}
	}
	//////////////////////
	vert_offset = 0;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			chunk = chunks[i][j];
			if (!chunk)
			{
				continue;
			}
			for (unsigned ii = 0; ii < 768; ii+=3)
			{
				tindices[ind_count] = chunk->GetIndices()[ii+2]+ vert_offset;
				tindices[ind_count + 1] = chunk->GetIndices()[ii+1] + vert_offset;
				tindices[ind_count + 2] = chunk->GetIndices()[ii] + vert_offset;
				ind_count += 3;
			}
			vert_offset += 145;
			for (auto &doodad : chunk->GetDoodads())
			{
				if (doodad.IsSkipped())
					continue;
				for (unsigned ii = 0; ii < doodad.GetIndexCount(); ii+=3)
				{
					tindices[ind_count] = doodad.GetIndices()[ii + 2] + vert_offset;
					tindices[ind_count + 1] = doodad.GetIndices()[ii + 1] + vert_offset;
					tindices[ind_count + 2] = doodad.GetIndices()[ii] + vert_offset;
					ind_count += 3;
				}
				vert_offset += doodad.GetVertexCount();
			}
			for (auto &wmo : chunk->GetWMOs())
			{
				if (wmo.IsSkipped())
					continue;
				for (auto &part : wmo.GetParts())
				{
					for (unsigned ii = 0; ii < part.GetIndexCount(); ii+=3)
					{
						tindices[ind_count] = part.GetIndices()[ii + 2] + vert_offset;
						tindices[ind_count + 1] = part.GetIndices()[ii + 1] + vert_offset;
						tindices[ind_count + 2] = part.GetIndices()[ii] + vert_offset;
						ind_count += 3;
					}
					vert_offset += part.GetVertexCount();
				}
			}
		}
	}
	ofstream file;
	file.open("test.obj");
	for (unsigned vi = 0; vi < vertex_count*3; vi+=3)
	{
		file << "v " << tvertices[vi] << " " << tvertices[vi + 1] << " " << tvertices[vi + 2] << endl;
	}
	for (unsigned ii = 0; ii < index_count; ii += 3)
	{
		file << "f " << tindices[ii]+1 << " " << tindices[ii + 1]+1 << " " << tindices[ii + 2]+1 << endl;

	}
	/*for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			chunk = chunks[i][j];
			if (!chunk)
			{
				continue;
			}


		}
	}*/
}

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

void SquareArea::InitNavigation2()
{
	unique_ptr<float> vertices;
	unique_ptr<int> indices;
	unsigned index_count = 0;
	unsigned vertex_count = 0;
	float x = 0;
	float y = 0;
	Chunk *chunk;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			chunk = chunks[i][j];
			if (!chunk) continue;
			vertex_count += chunk->vert_count;
			index_count += chunk->ind_count;
		}
	}
	vertices = unique_ptr<float>(new float[vertex_count*3]);
	indices = unique_ptr<int>(new int[index_count]);
	float * tvertices = vertices.get();
	int * tindices = indices.get();
	unsigned vert_offset = 0;
	unsigned ind_count = 0;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			chunk = chunks[i][j];
			if (!chunk) continue;
			for (unsigned vi = 0; vi < chunk->vert_count*3; vi ++)
			{
				tvertices[vert_offset++] = chunk->nav_vertices.get()[vi];
			}

		}
	}
	vert_offset = 0;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			chunk = chunks[i][j];
			if (!chunk) continue;
			for (unsigned ii = 0; ii < chunk->ind_count; ii++)
			{
				tindices[ind_count++] = chunk->nav_indices.get()[ii] + vert_offset;
			}
			vert_offset += chunk->vert_count;
		}
	}
	ofstream file;
	file.open("new_test.obj");
	for (unsigned vi = 0; vi < vertex_count * 3; vi += 3)
	{
		file << "v " << tvertices[vi] << " " << tvertices[vi + 1] << " " << tvertices[vi + 2] << endl;
	}
	for (unsigned ii = 0; ii < index_count; ii += 3)
	{
		file << "f " << tindices[ii] + 1 << " " << tindices[ii + 1] + 1 << " " << tindices[ii + 2] + 1 << endl;

	}

}

void SquareArea::InitChunkyMesh()
{

}

void SquareArea::InitNavigationR()
{
	InitAreaBoundingBox();
	InitNavigation2();
	BuildAllTiles();
	
}

void SquareArea::InitAreaBoundingBox()
{
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
				points.push_back(chunk->GetTerrainBoundingBox().up.y);
				points.push_back(chunk->GetTerrainBoundingBox().up.z);
				points.push_back(chunk->GetTerrainBoundingBox().down.x);
				points.push_back(chunk->GetTerrainBoundingBox().down.y);
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
	WowBuildContext *m_ctx = new WowBuildContext();
	m_tileSize = 32.0;
	m_cellSize = 0.300000012;
	m_maxTiles = 1024;
	m_maxPolysPerTile = 4096;
	const float* bmin = bounding_box.GetArrayMin();
	const float* bmax = bounding_box.GetArrayMax();
	int gw = 0, gh = 0;
	rcCalcGridSize(bmin, bmax, m_cellSize, &gw, &gh);
	const int ts = (int)m_tileSize;
	const int tw = (gw + ts - 1) / ts;
	const int th = (gh + ts - 1) / ts;
	const float tcs = m_tileSize*m_cellSize;
	float m_tileBmin[3];
	float m_tileBmax[3];
	dtFreeNavMesh(m_navMesh);

	m_navMesh = dtAllocNavMesh();
	if (!m_navMesh)
	{
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not allocate navmesh.");
		return;
	}
	dtNavMeshParams params;
	rcVcopy(params.orig, bmin);
	params.tileWidth = m_tileSize*m_cellSize;
	params.tileHeight = m_tileSize*m_cellSize;
	params.maxTiles = m_maxTiles;
	params.maxPolys = m_maxPolysPerTile;

	dtStatus status;
	//m_navMesh = new dtNavMesh();
	status = m_navMesh->init(&params);
	if (dtStatusFailed(status))
	{
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init navmesh.");
		return;
	}

	status = m_navQuery->init(m_navMesh, 2048);
	if (dtStatusFailed(status))
	{
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init Detour navmesh query");
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
		m_ctx->log(RC_LOG_ERROR, "buildTiledNavigation: Could not init Detour navmesh query");
		return;
	}
}

unsigned char * SquareArea::BuildTileMesh(int x, int y, const float* bmin, const float* bmax, int dataSize)
{
	
	WowBuildContext * m_ctx = new WowBuildContext();
	rcConfig m_cfg;
	memset(&m_cfg, 0, sizeof(m_cfg));
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
	m_cfg.tileSize = (int)m_tileSize;
	m_cfg.borderSize = m_cfg.walkableRadius + 3; // Reserve enough padding.
	m_cfg.width = m_cfg.tileSize + m_cfg.borderSize * 2;
	m_cfg.height = m_cfg.tileSize + m_cfg.borderSize * 2;
	m_cfg.detailSampleDist = m_detailSampleDist < 0.9f ? 0 : m_cellSize * m_detailSampleDist;
	m_cfg.detailSampleMaxError = m_cellHeight * m_detailSampleMaxError;

	rcVcopy(m_cfg.bmin, bmin);
	rcVcopy(m_cfg.bmax, bmax);
	m_cfg.bmin[0] -= m_cfg.borderSize*m_cfg.cs;
	m_cfg.bmin[2] -= m_cfg.borderSize*m_cfg.cs;
	m_cfg.bmax[0] += m_cfg.borderSize*m_cfg.cs;
	m_cfg.bmax[2] += m_cfg.borderSize*m_cfg.cs;

	rcHeightfield * m_solid = rcAllocHeightfield();
	if (!m_solid)
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'solid'.");
		return 0;
	}
	if (!rcCreateHeightfield(m_ctx, *m_solid, m_cfg.width, m_cfg.height, m_cfg.bmin, m_cfg.bmax, m_cfg.cs, m_cfg.ch))
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create solid heightfield.");
		return 0;
	}

	float tbmin[2], tbmax[2];
	tbmin[0] = m_cfg.bmin[0];
	tbmin[1] = m_cfg.bmin[2];
	tbmax[0] = m_cfg.bmax[0];
	tbmax[1] = m_cfg.bmax[2];
	vector<Chunk*> overlapping_chunks = vector<Chunk*>();
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
				m_triareas = new unsigned char[nctris];
				rcMarkWalkableTriangles(m_ctx, m_cfg.walkableSlopeAngle, verts, chunk->vert_count, ctris, nctris, m_triareas);
				if (!rcRasterizeTriangles(m_ctx, verts, chunk->vert_count, ctris, m_triareas, nctris, *m_solid, m_cfg.walkableClimb))
					return 0;
				delete[] m_triareas;
				m_triareas = 0;
			}

		}
	}
	rcFilterLowHangingWalkableObstacles(m_ctx, m_cfg.walkableClimb, *m_solid);
	rcFilterLedgeSpans(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid);
	rcFilterWalkableLowHeightSpans(m_ctx, m_cfg.walkableHeight, *m_solid);

	rcCompactHeightfield * m_chf = rcAllocCompactHeightfield();
	if (!m_chf)
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'chf'.");
		return 0;
	}
	if (!rcBuildCompactHeightfield(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid, *m_chf))
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build compact data.");
		return 0;
	}

	rcFreeHeightField(m_solid);
	m_solid = 0;

	if (!rcErodeWalkableArea(m_ctx, m_cfg.walkableRadius, *m_chf))
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not erode.");
		return 0;
	}

	if (m_partitionType == SAMPLE_PARTITION_WATERSHED)
	{
		// Prepare for region partitioning, by calculating distance field along the walkable surface.
		if (!rcBuildDistanceField(m_ctx, *m_chf))
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build distance field.");
			return false;
		}

		// Partition the walkable surface into simple regions without holes.
		if (!rcBuildRegions(m_ctx, *m_chf, m_cfg.borderSize, m_cfg.minRegionArea, m_cfg.mergeRegionArea))
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build watershed regions.");
			return false;
		}
	}

	rcContourSet * m_cset = rcAllocContourSet();
	if (!m_cset)
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'cset'.");
		return 0;
	}
	if (!rcBuildContours(m_ctx, *m_chf, m_cfg.maxSimplificationError, m_cfg.maxEdgeLen, *m_cset))
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create contours.");
		return 0;
	}

	if (m_cset->nconts == 0)
	{
		return 0;
	}

	// Build polygon navmesh from the contours.
	rcPolyMesh *  m_pmesh = rcAllocPolyMesh();
	if (!m_pmesh)
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmesh'.");
		return 0;
	}
	
	if (!rcBuildPolyMesh(m_ctx, *m_cset, m_cfg.maxVertsPerPoly, *m_pmesh))
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not triangulate contours.");
		return 0;
	}
	polys.push_back(m_pmesh);
	// Build detail mesh.
	rcPolyMeshDetail * m_dmesh = rcAllocPolyMeshDetail();
	if (!m_dmesh)
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'dmesh'.");
		return 0;
	}

	if (!rcBuildPolyMeshDetail(m_ctx, *m_pmesh, *m_chf,
		m_cfg.detailSampleDist, m_cfg.detailSampleMaxError,
		*m_dmesh))
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could build polymesh detail.");
		return 0;
	}

	rcFreeCompactHeightfield(m_chf);
	m_chf = 0;
	rcFreeContourSet(m_cset);
	m_cset = 0;

	unsigned char* navData = 0;
	int navDataSize = 0;
	if (m_cfg.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
	{
		if (m_pmesh->nverts >= 0xffff)
		{
			// The vertex indices are ushorts, and cannot point to more than 0xffff vertices.
			m_ctx->log(RC_LOG_ERROR, "Too many vertices per tile %d (max: %d).", m_pmesh->nverts, 0xffff);
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
		params.offMeshConVerts =0;
		params.offMeshConRad = 0;
		params.offMeshConDir = 0;
		params.offMeshConAreas =0;
		params.offMeshConFlags = 0;
		params.offMeshConUserID = 0;
		params.offMeshConCount = 0;
		params.walkableHeight = m_agentHeight;
		params.walkableRadius = m_agentRadius;
		params.walkableClimb = m_agentMaxClimb;
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
			m_ctx->log(RC_LOG_ERROR, "Could not build Detour navmesh.");
			return 0;
		}
	}
	dataSize = navDataSize;
	return navData;
}

int SquareArea::FindPath(Vector3 & start, Vector3 & end, int nPathSlot)
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
	status = m_navQuery->findNearestPoly((float*)&start, pExtents, &Filter, &StartPoly, StartNearest);
	if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK)) return -1; // couldn't find a polygon

																		  // find the end polygon
	status = m_navQuery->findNearestPoly((float*)&end, pExtents, &Filter, &EndPoly, EndNearest);
	if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK)) return -2; // couldn't find a polygon

	status = m_navQuery->findPath(StartPoly, EndPoly, StartNearest, EndNearest, &Filter, PolyPath, &nPathCount, MAX_PATHPOLY);
	if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK)) return -3; // couldn't create a path
	if (nPathCount == 0) return -4; // couldn't find a path

	status = m_navQuery->findStraightPath(StartNearest, EndNearest, PolyPath, nPathCount, StraightPath, NULL, NULL, &nVertCount, MAX_PATHVERT + 100);
	if ((status&DT_FAILURE) || (status&DT_STATUS_DETAIL_MASK)) return -5; // couldn't create a path
	if (nVertCount == 0) return -6; // couldn't find a path

									// At this point we have our path.  Copy it to the path store
	int nIndex = 0;
	for (int nVert = 0; nVert<nVertCount; nVert++)
	{
		/*m_PathStore[nPathSlot].PosX[nVert] = StraightPath[nIndex++];
		m_PathStore[nPathSlot].PosY[nVert] = StraightPath[nIndex++];
		m_PathStore[nPathSlot].PosZ[nVert] = StraightPath[nIndex++];*/

		m_PathStore.PosX[nVert] = StraightPath[nIndex++];
		m_PathStore.PosY[nVert] = StraightPath[nIndex++];
		m_PathStore.PosZ[nVert] = StraightPath[nIndex++]; 

		//sprintf(m_chBug, "Path Vert %i, %f %f %f", nVert, m_PathStore[nPathSlot].PosX[nVert], m_PathStore[nPathSlot].PosY[nVert], m_PathStore[nPathSlot].PosZ[nVert]) ;
		//m_pLog->logMessage(m_chBug);
	}
	m_PathStore.MaxVertex = nVertCount;
	//m_PathStore[nPathSlot].Target = nTarget;
	return nVertCount;
}

void SquareArea::InitWMOs()
{
	wmos.clear();
	active_wmos.clear();
	int c = 0;
	bool exist = false;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			if (chunks[i][j])
				for (auto & wmo : chunks[i][j]->GetWMOs())
				{
					exist = false;
					for (auto & check_wmo : wmos)
					{
						if (wmo.GetUUID() == check_wmo->GetUUID() && &wmo != check_wmo)
						{
							exist = true;
							break;
						}
					}
					//if (!exist)
						wmos.push_back(&wmo);	
					
				}
		}
	}
}

void SquareArea::InitDoodads()
{
	doodads.clear();
	active_doodads.clear();
	int c = 0;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			if (chunks[i][j])
				for (auto & doodad : chunks[i][j]->GetDoodads())
				{
					doodads.push_back(&doodad);
				}
		}
	}
}
