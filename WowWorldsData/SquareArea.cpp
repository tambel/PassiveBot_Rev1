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
	/*for (auto &chunk : chunkss)
	{
		chunk->HitUnuseed();
		chunk->Free();
	}
	for (vector<unique_ptr<Chunk>>::iterator it = chunkss.begin(); it != chunkss.end();)
	{
		if ((*it)->IsAlive())
			++it;
		else
			it = chunkss.erase(it);
	}*/
	WMO * wmo;
	for (auto &wmo_ptr : wmos)
	{
		wmo = wmo_ptr.get();
		wmo->HitUnuseed();
		wmo->Free();
		wmo->Unoccupie();

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
			doodad->Unoccupie();

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


Area::~Area(void)
{

}

Area & Area::operator=(Area && right)
{
	doodads = move(right.doodads);
	wmos = move(right.wmos);
	chunks = right.chunks;
	chunkss = move(right.chunkss);
	active_chunks = move(right.active_chunks);
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
 	active_chunks.clear();
	this->location = location;
	this->block_coordinates = block_coordinates;
	this->coordinates = coordinates;
	int area_size = radius * 2 + 1;
	Point2D<int> area_position = (coordinates + Point2D<int>(16, 16)) - Point2D<int>(radius, radius);
	Vector3 block_real_position = Vector3(block_coordinates.Y*Metrics::BlockSize, -block_coordinates.X*Metrics::BlockSize, 0.0f);


	bool exist;
	for (auto &chunk : chunkss)
		chunk->ToRemove();
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			exist = false;
			Point2D<int> abs_pos = area_position + Point2D<int>(i, j);
			Point2D<int>bc = block_coordinates + (Point2D<int>(abs_pos.X / 16, abs_pos.Y / 16) - Point2D<int>(1, 1));
			Point2D<int> c = Point2D<int>(abs_pos.X % 16, abs_pos.Y % 16);
			//chunks[i][j] = ADTWorker::GetChunk(this,location, block_coordinates + (Point2D<int>(abs_pos.X / 16, abs_pos.Y / 16) - Point2D<int>(1, 1)), Point2D<int>(abs_pos.X % 16, abs_pos.Y % 16));
			
			

			for (auto &chunk : chunkss)
			{
				if (chunk->GetLocation() == location && chunk->GetBlockCoordinates() == bc && chunk->GetCoordinates() == c)
				{

					chunk->Prolong();
					exist = true;
					break;
					/*chunk->Refresh();
					active_chunks.push_back(&*chunk);
					exist = true;
					break;*/
				}
				
			}
			if (!exist)
			{
				Chunk * chunk = ADTWorker::GetChunk(this, location, bc, c);
				//unique_ptr<Chunk> chunk_ptr = unique_ptr<Chunk>(ADTWorker::GetChunk(this, location, bc, c));
				if (chunk)
				{
					//chunk->Refresh();
					chunkss.push_back(unique_ptr<Chunk>(chunk));
					chunk->Prolong();
					//active_chunks.push_back(chunk);
				}
			}

		}

	}
	for (vector<unique_ptr<Chunk>>::iterator it = chunkss.begin(); it != chunkss.end();)
	{
		if ((*it)->IsRemoved())
			it = chunkss.erase(it);
		else
			++it;
	}
	InitMapObjects();
	//CheckAndClearOldObjects();
	//for (auto ac : active_chunks)
		//ac->InitObjects();
	
	cout << "WMOS" << endl;
	for (auto &wmo : wmos)
	{
		cout << wmo->GetName() << ", " << wmo->GetUUID() << " Lifetime - "<<wmo->GetUnusedIterCount()<< endl;
	}

	//cout << "CHUNKS " << "WMOS" << endl;
	//for (int i = 0; i < area_size; i++)
	//{
	//	for (int j = 0; j < area_size; j++)
	//	{
	//		Chunk * chunk = chunks[i][j];
	//		if (!chunk) continue;
	//		for (auto wmo : chunk->GetWMOs())
	//		{
	//			cout << wmo->GetName() << ", " << wmo->GetUUID() << endl;
	//		}
	//	}
	//}
	
}

void Area::CheckAndMove(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates)
{
	if (IsOutOfBounds(location, block_coordinates, coordinates))
		Fill(location, block_coordinates, coordinates);
	/*if (IsMoved(location, block_coordinates, coordinates))
	{
		Fill(location, block_coordinates, coordinates);
	}*/

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

void Area::InitAreaBoundingBox()
{
	vector<float> points = vector<float>();
	Chunk * chunk;
	auto add_point = [](vector<float> & points, Utils::Graphics::BoundingBox & bb)
	{
		points.push_back(bb.up.x);
		points.push_back(bb.up.y);
		points.push_back(bb.up.z);
		points.push_back(bb.down.x);
		points.push_back(bb.down.y);
		points.push_back(bb.down.z);
	};
	/*for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			chunk = chunks[i][j];
			if (chunk)*/
	for (auto &chunk : chunkss)
	{
		add_point(points, chunk->GetBoundingBox());
	}
	rcCalcBounds(&points[0], points.size() / 3, bounding_box.GetArrayMin(), bounding_box.GetArrayMax());
	points.clear();
	/*for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			chunk = chunks[i][j];
			if (chunk)*/

	for (auto &wmo : wmos)
	{

		add_point(points, wmo->GetBoundingBox());
	}
	for (auto &doodad : doodads)
	{

		add_point(points, doodad->GetBoundingBox());
	}
	//add_point(points, chunk->GetBoundingBox());


	Utils::Graphics::BoundingBox bb;
	if (points.size() > 0)
	{
		rcCalcBounds(&points[0], points.size() / 3, bb.GetArrayMin(), bb.GetArrayMax());
		bounding_box.up.y = bb.up.y;
		bounding_box.down.y = bb.down.y;
	}

}

bool Area::IsOutOfBounds(Location * location, Point2D<int> block_coordinates,Point2D<int> coordinates)
{
	if (this->location == location)
	{
		Point2D<int> global_coords = Utils::ToGlobalChunkCoords(this->block_coordinates, this->coordinates);
		Point2D<int> target_global_coords = Utils::ToGlobalChunkCoords(block_coordinates, coordinates);
		if (abs(target_global_coords.X - global_coords.X) > radius || abs(target_global_coords.Y - global_coords.Y) > radius)
			return true;
		else
			return false;
	}
	return true;
}

void Area::InitMapObjects()
{
	//wmos
	bool exist;
	for (auto &wmo : wmos)
		wmo->ToRemove();

	for (auto &chunk : chunkss)
	{
		for (auto wmo_info : chunk->GetWMOInfos())
		{
			exist = false;
			for (auto &wmo : wmos)
			{
				if (wmo->GetUUID() == wmo_info.modf.UniqueId)
				{
					exist = true;
					wmo->Prolong();
					break;
				}
			}
			if (!exist)
			{
				WMO  * wmo = new WMO(wmo_info.filename, wmo_info.modf.UniqueId, Position(wmo_info.modf.Position, wmo_info.modf.Rotation));
				wmos.push_back(unique_ptr<WMO>(wmo));
				wmos.back()->Prolong();
			}

		}
	}
	//doodads
	for (auto &doodad : doodads)
		doodad->ToRemove();

	for (auto &chunk : chunkss)
	{
		for (auto doodad_info : chunk->GetDoodadInfos())
		{
			exist = false;
			for (auto &doodad : doodads)
			{
				if (doodad->GetUUID() == doodad_info.mddf.UniqueId)
				{
					exist = true;
					doodad->Prolong();
					break;
				}
			}
			if (!exist)
			{
				Doodad  * doodad = new Doodad(doodad_info.filename, doodad_info.mddf.UniqueId, Position(doodad_info.mddf.Position, doodad_info.mddf.Rotation),doodad_info.mddf.Scale);
				doodads.push_back(unique_ptr<Doodad>(doodad));
				doodads.back()->Prolong();
			}

		}
	}
	//removind unused objects
	//wmos
	for (vector<unique_ptr<WMO>>::iterator it = wmos.begin(); it != wmos.end();)
	{
		if ((*it)->IsRemoved())
			it = wmos.erase(it);
		else
			++it;	
	}
	//doodads
	for (vector<unique_ptr<Doodad>>::iterator it = doodads.begin(); it != doodads.end();)
	{
		if ((*it)->IsRemoved())
			it = doodads.erase(it);
		else
			++it;
	}
}
