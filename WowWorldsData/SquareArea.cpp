#include "stdafx.h"
#include <algorithm>

bool Area::IsMoved(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates)
{
	if (this->location->id != location->id || this->block_coordinates != block_coordinates || this->coordinates != coordinates)
	{
		return true;
	}
	return false;
}

void Area::CheckAndClearOldObjects()
{
	WMO * wmo;
	for (auto &wmo_ptr : wmos)
	{
		wmo = wmo_ptr.get();
		wmo->HitUnuseed();
		wmo->Free();

	}
	for (vector<unique_ptr<WMO>>::iterator it = wmos.begin(); it != wmos.end();)
	{
		if (it->get()->IsAlive())
			++it;
		else
			it = wmos.erase(it);
	}
	Doodad * doodad;
	{
		for (auto &doodad_ptr : doodads)
		{
			doodad = doodad_ptr.get();
			doodad->HitUnuseed();
			doodad->Free();

		}
		for (vector<unique_ptr<Doodad>>::iterator it = doodads.begin(); it != doodads.end();)
		{
			if (it->get()->IsAlive())
				++it;
			else
				it = doodads.erase(it);
		}
	}
}

Area::Area()
{
}
Area::Area(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius) :location(location), block_coordinates(block_coordinates), coordinates(coordinates), radius(radius)
{
	busy = false;
	area_size = radius * 2 + 1;
	chunks = new Chunk**[area_size];
	wow_object_avatars = vector<WowObjectAvatar*>();
	for (int i = 0; i < area_size; i++)
	{
		chunks[i] = new Chunk *[area_size];
		for (int j = 0; j < area_size; j++)
		{
			chunks[i][j] = 0;
		}
	}
}

//Area::Area(Area && area)
//{
//
//}


Area::~Area(void)
{

}

Area & Area::operator=(Area && right)
{
	wmos = move(right.wmos);
	chunks = right.chunks;
	right.chunks = nullptr;
	radius = right.radius;
	right.radius = 0;
	area_size = right.area_size;
	right.area_size = 0;
	location = right.location;
	right.location = nullptr;
	block_coordinates = right.block_coordinates;
	coordinates = right.coordinates;
	bounding_box = right.bounding_box;
	to_update = right.to_update;
	busy = right.busy;
	

	return *this;
}

void Area::Fill(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates)
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

			chunks[i][j] = ADTWorker::GetChunk(this,location, block_coordinates += (Point2D<int>(abs_pos.X / 16, abs_pos.Y / 16) - Point2D<int>(1, 1)), Point2D<int>(abs_pos.X % 16, abs_pos.Y % 16));

		}

	}
	//DeleteDuplicates();
	CheckAndClearOldObjects();
	
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			//Point2D<int> abs_pos = area_position + Point2D<int>(i, j);

			//chunks[i][j] = ADTWorker::GetChunk(location, block_coordinates += (Point2D<int>(abs_pos.X / 16, abs_pos.Y / 16) - Point2D<int>(1, 1)), Point2D<int>(abs_pos.X % 16, abs_pos.Y % 16));
			if (!chunks[i][j]) continue;
			chunks[i][j]->InitNavigation();
		}

	}


	
}

void Area::CheckAndMove(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates)
{
	if (IsMoved(location, block_coordinates, coordinates))
	{
		Fill(location, block_coordinates, coordinates);
	}

}
void Area::AddWowObjectAvatar(Wow::WowObject * object)
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

void Area::DeleteDuplicates()
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
						if (uuid == wmo->GetUUID())
						{
							wmo->Skip();
							break;
						}
					}
					if (wmo->IsSkipped())
						continue;
					wmo_uuids.push_back(wmo->GetUUID());

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


void Area::InitAreaBoundingBox()
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
				points.push_back(chunk->GetFullBoundingBox().up.x);
				points.push_back(chunk->GetFullBoundingBox().up.y);
				points.push_back(chunk->GetFullBoundingBox().up.z);
				points.push_back(chunk->GetFullBoundingBox().down.x);
				points.push_back(chunk->GetFullBoundingBox().down.y);
				points.push_back(chunk->GetFullBoundingBox().down.z);
				/*points.push_back(chunk->GetTerrainBoundingBox().up.x);
				points.push_back(chunk->GetFullBoundingBox().up.y);
				points.push_back(chunk->GetTerrainBoundingBox().up.z);
				points.push_back(chunk->GetTerrainBoundingBox().down.x);
				points.push_back(chunk->GetFullBoundingBox().down.y);
				points.push_back(chunk->GetTerrainBoundingBox().down.z);*/

			}
			
		}
	}
	rcCalcBounds(&points[0], points.size() / 3, bounding_box.GetArrayMin(), bounding_box.GetArrayMax());

}


void Area::ToMesh()
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