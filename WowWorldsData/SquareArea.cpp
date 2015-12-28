#include "stdafx.h"
#include <algorithm>
SquareArea::SquareArea(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius) :location(location), block_coordinates(block_coordinates), coordinates(coordinates), radius(radius)
{
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
}


SquareArea::~SquareArea(void)
{
}

void SquareArea::Fill(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates)
{
	this->location = location;
	this->block_coordinates = block_coordinates;
	this->coordinates = coordinates;
	int area_size = radius * 2 + 1;
	Point2D<int> area_position = (coordinates + Point2D<int>(16, 16)) - Point2D<int>(radius, radius);
	Vector3 block_real_position = Vector3(block_coordinates.Y*Metrics::BlockSize, -block_coordinates.X*Metrics::BlockSize, 0.0f);
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			Point2D<int> abs_pos = area_position + Point2D<int>(i, j);

			chunks[i][j] = ADTWorker::GetChunk(location, block_coordinates += (Point2D<int>(abs_pos.X / 16, abs_pos.Y / 16) - Point2D<int>(1, 1)), Point2D<int>(abs_pos.X % 16, abs_pos.Y % 16), true);
		}

	}
	InitBoundingBox();

}

void SquareArea::Move(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates)
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
			if (block_changed)
				InitObjects();
			InitActiveObjects();
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
		if (bounding_box.IsInside2D(doodad->GetPosition().coords))
		{
			active_doodads.push_back(doodad);
		}
	}
	active_wmos.clear();
	for (auto &wmo : wmos)
	{
		if (bounding_box.IsInside2D(wmo->GetPosition().coords))
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
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			if (bounding_box.up.x > chunks[i][j]->GetRealPosition().x)
			{
				bounding_box.up.x = chunks[i][j]->GetRealPosition().x;
			}
			if (bounding_box.up.y > chunks[i][j]->GetRealPosition().y)
			{

				bounding_box.up.y = chunks[i][j]->GetRealPosition().y;
			}
		}
	}
	bounding_box.down = bounding_box.up + Vector3(area_size*Metrics::ChunkSize, area_size*Metrics::ChunkSize, 0);
}

void SquareArea::InitWMOs()
{
	wmos.clear();
	for (int i = block_coordinates.X - 1; i <= block_coordinates.X + 1; i++)
	{
		for (int j = block_coordinates.Y - 1; j <= block_coordinates.Y + 1; j++)
		{
			ADT * adt = ADTWorker::GetADT(location, Point2D<int>(i, j));
			if (!adt)
				continue;
			for (auto wmo_info : adt->GetWMOInfos())
			{
				bool exist = false;
				for (auto &old_wmo : old_wmos)
				{
					if (old_wmo)
					{
						if (old_wmo->GetUUID() == wmo_info.modf.UniqueId)
						{
							exist = true;
							wmos.push_back(old_wmo);
							old_wmo = 0;
							break;
						}
					}
				}
				if (exist)
				{
					//continue;
				}

				WMORoot  wmo_root = WMORoot(wmo_info.filename);
				WMO * wmo = new  WMO(wmo_root, wmo_info.modf.UniqueId, Position(wmo_info.modf.Position, wmo_info.modf.Rotation));
				wmos.push_back(wmo);
			}
		}
	}
	int count = 0;
	for (auto &wmo : wmos)
	{
		for (auto &wmo2 : wmos)
		{
			if (wmo && wmo2)
			{
				if (wmo->GetUUID() == wmo2->GetUUID() && wmo != wmo2)
				{
					count++;
					wmo = 0;
				}
			}
		}
	}
	wmos.erase(remove(wmos.begin(), wmos.end(), (WMO*)0), wmos.end());
	for (auto old_wmo : old_wmos)
	{
		cout << " Init Old" << endl;
		delete old_wmo;

	}
	old_wmos.clear();
	for (auto wmo : wmos)
	{
		old_wmos.push_back(wmo);
	}
}

void SquareArea::InitDoodads()
{
	doodads.clear();
	for (int i = block_coordinates.X - 1; i <= block_coordinates.X + 1; i++)
	{
		for (int j = block_coordinates.Y - 1; j <= block_coordinates.Y + 1; j++)
		{
			ADT * adt = ADTWorker::GetADT(location, Point2D<int>(i, j));
			if (!adt)
				continue;
			for (auto m2_info : *adt->GetM2Infos())
			{
				bool exist = false;
				bool duplicate = false;
				for (auto &old_doodad : old_doodads)
				{
					if (old_doodad)
					{
						if (old_doodad->GetUUID() == m2_info.mddf.UniqueId)
						{
							exist = true;
							doodads.push_back(old_doodad);
							old_doodad = 0;

							break;
						}
					}
				}
				if (exist)
				{
					continue;
				}

				//M2 * m2 = new M2(m2_info.file);
				//Doodad * doodad = new Doodad;
				Doodad * doodad = new Doodad(m2_info.file, m2_info.mddf.UniqueId, Position(m2_info.mddf.Position, m2_info.mddf.Rotation), m2_info.mddf.Scale);
				doodads.push_back(doodad);
				//delete m2;
			}
		}
	}
	int count = 0;
	for (auto doodad : doodads)
	{
		for (auto &doodad2 : doodads)
		{
			if (doodad && doodad2)
			{
				if (doodad->GetUUID() == doodad2->GetUUID() && doodad != doodad2)
				{
					count++;
					doodad2 = 0;
				}
			}
		}
	}
	doodads.erase(remove(doodads.begin(), doodads.end(), (Doodad*)0), doodads.end());
	for (auto old_doodad : old_doodads)
	{
		delete old_doodad;
	}
	old_doodads.clear();
	for (auto doodad : doodads)
	{
		old_doodads.push_back(doodad);
	}
}
