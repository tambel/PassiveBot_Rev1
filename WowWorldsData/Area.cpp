#include "stdafx.h"
#include <algorithm>


Area::Area()
{
}
Area::Area(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius) :location(location), block_coordinates(block_coordinates), coordinates(coordinates), radius(radius)
{
	area_size = radius * 2 + 1;
	this->Update(location,block_coordinates,coordinates);
}
Area::~Area(void)
{

}

Area & Area::operator=(Area && right)
{
	_move(right);
	

	return *this;
}

Area::Area(Area && area)
{
	_move(area);
}

void Area::Update(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates)
{
	ADTWorker::Clear();
	this->location = location;
	this->block_coordinates = block_coordinates;
	this->coordinates = coordinates;
	int area_size = radius * 2 + 1;
	Point2D<int> area_position = (coordinates + Point2D<int>(16, 16)) - Point2D<int>(radius, radius);
	Vector3 block_real_position = Vector3(block_coordinates.Y*Metrics::BlockSize, -block_coordinates.X*Metrics::BlockSize, 0.0f);


	bool exist;
	for (auto &chunk : chunks)
		chunk->ToRemove();
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			exist = false;
			Point2D<int> abs_pos = area_position + Point2D<int>(i, j);
			Point2D<int>bc = block_coordinates + (Point2D<int>(abs_pos.X / 16, abs_pos.Y / 16) - Point2D<int>(1, 1));
			Point2D<int> c = Point2D<int>(abs_pos.X % 16, abs_pos.Y % 16);
			for (auto &chunk : chunks)
			{
				if (chunk->GetLocation() == location && chunk->GetBlockCoordinates() == bc && chunk->GetCoordinates() == c)
				{

					chunk->Prolong();
					exist = true;
					break;
				}
				
			}
			if (!exist)
			{
				Chunk * chunk = ADTWorker::GetChunk(this, location, bc, c);
				if (chunk)
				{
					chunks.push_back(unique_ptr<Chunk>(chunk));
					chunk->Prolong();
				}
			}

		}

	}
	for (vector<unique_ptr<Chunk>>::iterator it = chunks.begin(); it != chunks.end();)
	{
		if ((*it)->IsRemoved())
			it = chunks.erase(it);
		else
			++it;
	}
	if (chunks.size() == 0)
	{
		throw(EmptyAreaException());
	}
	chunks.shrink_to_fit();
	InitMapObjects();
	
}

void Area::_move(Area & other)
{
	doodads = move(other.doodads);
	wmos = move(other.wmos);
	chunks = move(other.chunks);
	radius = other.radius;
	other.radius = 0;
	area_size = other.area_size;
	other.area_size = 0;
	location = other.location;
	block_coordinates = other.block_coordinates;
	coordinates = other.coordinates;
	bounding_box = other.bounding_box;
}

void Area::CheckAndUpdate(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates)
{
	if (IsOutOfBounds(location, block_coordinates, coordinates))
	{
	
		this->Update(location, block_coordinates, coordinates);
	}


}

bool Area::IsOutOfBounds(Location & location, Point2D<int> block_coordinates,Point2D<int> coordinates)
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

	for (auto &chunk : chunks)
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
	for (auto &doodad : doodads)
		doodad->ToRemove();

	for (auto &chunk : chunks)
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

