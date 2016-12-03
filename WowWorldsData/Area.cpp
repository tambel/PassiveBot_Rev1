#include "stdafx.h"
#include <algorithm>
#include "Area.h"

#ifdef BOTDEV
#include <conio.h>
#endif

using namespace Utils;
using namespace Utils::Game;

Area::Area()
{
}
Area::Area(int radius, AreaFormat format)
{
	this->radius = radius;
	this->format = format;
}
Area::Area(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius) :location(location), block_coordinates(block_coordinates), coordinates(coordinates), radius(radius)
{
	a = MMM();
	area_size = radius * 2 + 1;
	this->Update(location,block_coordinates,coordinates);
}
Area::~Area(void)
{

}

//Area & Area::operator=(Area && right)
//{
//	_move(right);
//	
//
//	return *this;
//}

//Area::Area(Area && area)
//{
//	_move(area);
//}

void Area::Update(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates)
{
	format = AreaFormat::fBlock;
	ADTWorker::Clear();
	this->location = location;
	this->block_coordinates = block_coordinates;
	this->coordinates = coordinates;
	area_size = radius * 2 + 1;
	std::for_each(chunks.begin(), chunks.end(), [](unique_ptr<Chunk> & chunk) {chunk->ToRemove(); });
	if (format == AreaFormat::fBlock)
		UpdateCetralizedBlockScale(location, block_coordinates);
	else if (format == AreaFormat::fChunk)
		UpdateCetralizedChunkScale(location, block_coordinates, coordinates);



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



	InitMapObjects();
	InitAreaBoundingBox();
	//ToMesh();
	
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
#ifdef BOTDEV
	doodads.clear();
#endif
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
#ifdef BOTDEV
				Vector3 rot = doodad_info.mddf.Rotation;
				rot = Vector3(rot.x + rx, rot.y + ry, rot.z + rz);
				Doodad  * doodad = new Doodad(doodad_info.filename, doodad_info.mddf.UniqueId, Position(doodad_info.mddf.Position, rot), doodad_info.mddf.Scale);
#else
				Doodad  * doodad = new Doodad(doodad_info.filename, doodad_info.mddf.UniqueId, Position(doodad_info.mddf.Position, doodad_info.mddf.Rotation),doodad_info.mddf.Scale);
#endif
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

Vector3 Area::GetCenter()
{
	return Vector3(bounding_box.up.x + (bounding_box.down.x - bounding_box.up.x) / 2, bounding_box.up.y + (bounding_box.down.y - bounding_box.up.y) / 2, bounding_box.up.z + (bounding_box.down.z - bounding_box.up.z) / 2);
}

void Area::UpdateCetralizedBlockScale(Location & location, Point2D<int> block_coordinates)
{
	for (int x = block_coordinates.X - radius; x <= block_coordinates.X + radius; x++)
	{
		for (int y = block_coordinates.Y - radius; y <= block_coordinates.Y + radius; y++)
		{
			if (x < 0 || y < 0)
				continue;
			for (int cx = 0; cx < 16; cx++)
			{
				for (int cy = 0; cy < 16; cy++)
				{
					AddChunk(location, Point2DI(x, y), Point2DI(cx, cy));
				}
			}

		}
	}
}

void Area::AddChunk(Location & location, Point2D<int> & block_coordinates, Point2D<int> & coordinates)
{
	bool exist = false;
	for (auto &chunk : chunks)
	{
		if (chunk->GetLocation() == location && chunk->GetBlockCoordinates() == block_coordinates && chunk->GetCoordinates() == coordinates)
		{

			chunk->Prolong();
			exist = true;
			break;
		}

	}
	if (!exist)
	{
		Chunk * chunk = ADTWorker::GetChunk(this, location, block_coordinates, coordinates);
		if (chunk)
		{
			chunks.push_back(unique_ptr<Chunk>(chunk));
			chunk->Prolong();
		}
	}
}

void Area::UpdateCetralizedChunkScale(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates)
{
	this->coordinates = coordinates;
	Point2D<int> area_position = (coordinates + Point2D<int>(16, 16)) - Point2D<int>(radius, radius);
	Vector3 block_real_position = Vector3(block_coordinates.Y*Metrics::BlockSize, -block_coordinates.X*Metrics::BlockSize, 0.0f);
	for (auto &chunk : chunks)
		chunk->ToRemove();
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			Point2D<int> abs_pos = area_position + Point2D<int>(i, j);
			Point2D<int>bc = block_coordinates + (Point2D<int>(abs_pos.X / 16, abs_pos.Y / 16) - Point2D<int>(1, 1));
			Point2D<int> c = Point2D<int>(abs_pos.X % 16, abs_pos.Y % 16);
			AddChunk(location, bc, c);

		}

	}
}

void Area::InitAreaBoundingBox()
{
	vector<float> points = vector<float>();
	auto add_point = [&points](Utils::Graphics::BoundingBox & bb)
	{
		points.push_back(bb.up.x);
		points.push_back(bb.up.y);
		points.push_back(bb.up.z);
		points.push_back(bb.down.x);
		points.push_back(bb.down.y);
		points.push_back(bb.down.z);
	};
	for (auto &chunk : chunks)
	{
		add_point(chunk->GetBoundingBox());
	}
	rcCalcBounds(&points[0], points.size() / 3, bounding_box.GetArrayMin(), bounding_box.GetArrayMax());
	points.clear();
	for (auto &wmo : wmos)
	{

		add_point(wmo->GetBoundingBox());
	}
	for (auto &doodad : doodads)
	{

		add_point(doodad->GetBoundingBox());
	}
	//add_point(points, chunk->GetBoundingBox());


	Utils::Graphics::BoundingBox bb;
	if (points.size() > 0)
	{
		rcCalcBounds(&points[0], points.size() / 3, bb.GetArrayMin(), bb.GetArrayMax());
		bounding_box.up.y = bb.up.y>bounding_box.up.y? bounding_box.up.y: bb.up.y;
		bounding_box.down.y = bb.down.y<bounding_box.down.y? bounding_box.down.y: bb.down.y;
	}

}
#ifdef BOTDEV
void Area::ToMesh()
{
	ofstream file("area.obj");
	unsigned offset = 0;
	auto write_model_verices = [& file](Model * model)
	{
		unsigned vcount = model->GetVertexCount();
		float * vert = model->GetVertices();
		for (unsigned i = 0; i < vcount*3; i += 3)
		{
			file <<"v "<< vert[i] << " " << vert[i + 1] << " " << vert[i + 2]<<endl;
		}
	};
	auto write_model_indices = [&offset, &file](Model * model)
	{
		unsigned vcount = model->GetVertexCount();
		unsigned ind_count = model->GetIndexCount();
		int * ind = model->GetIndices();
		for (unsigned i = 0; i < ind_count; i += 3)
		{
			file <<"f "<< offset+ ind[i]+1 << " " << offset + ind[i + 1]+1 << " " << offset + ind[i+2]+1 << endl;
		}
		offset += vcount;
	};
	for (auto & chunk : chunks)
	{
		write_model_verices(&*chunk);
	}
	for (auto & wmo : wmos)
	{
		write_model_verices(&*wmo);
	}
	for (auto & doodad : doodads)
	{
		write_model_verices(&*doodad);
	}

	for (auto & chunk : chunks)
	{
		write_model_indices (&*chunk);
	}
	for (auto & wmo : wmos)
	{
		write_model_indices(&*wmo);
	}
	for (auto & doodad : doodads)
	{
		write_model_indices(&*doodad);
	}
	file.close();
}

void Area::Rotate()
{
	int axis = 0;
	float angle = 0;
	while (1)
	{
		for (int x = 0; x < 8; x++)
			for (int y = 0; y < 8; y++)
				for (int z = 0; z < 8; z++)

				{
					getch();
					rx = -360 + x * 90;
					ry = -360 + y * 90;
					rz = -360 + z * 90;
					Update(LocationBase::Get("Kalimdor"), Point2DI(36, 35), Point2DI(7, 9));
					cout << rx << " " << ry << " " << rz << endl;
				}
	}

}

void Area::Rotate90()
{
	int axis = 0;
	float angle = 0;
	while (1)
	{

		int c = getch();
		cout << c << endl;
		switch (c)
		{
		case 97:
			axis = 1;
			angle = -90.0;
			break;
		case 100:
			axis = 1;
			angle = 90.0;
			break;
		case 119:
			axis = 2;
			angle = -90.0;
			break;
		case 115:
			axis = 2;
			angle = 90.0;
			break;
		case 52:
			axis = 0;
			angle = -90.0;
			break;
		case 54:
			axis = 0;
			angle = 90.0;
			break;
		default:
			break;
		}
		if (angle != 0)
		{
			if (axis == 0)
				rx += angle;
			else if (axis == 1)
				ry += angle;
			else
				rz += angle;
			Update(LocationBase::Get("Kalimdor"), Point2DI(36, 35), Point2DI(7, 9));
			cout << rx << " " << ry << " " << rz << endl;
		}
	}
}

#endif