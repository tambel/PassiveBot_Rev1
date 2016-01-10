#include "stdafx.h"
#include <algorithm>
SquareArea::SquareArea()
{
}
SquareArea::SquareArea(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates, int radius) :location(location), block_coordinates(block_coordinates), coordinates(coordinates), radius(radius)
{
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
	navigation.SetArea(*this);
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
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			Point2D<int> abs_pos = area_position + Point2D<int>(i, j);

			chunks[i][j] = ADTWorker::GetChunk(location, block_coordinates += (Point2D<int>(abs_pos.X / 16, abs_pos.Y / 16) - Point2D<int>(1, 1)), Point2D<int>(abs_pos.X % 16, abs_pos.Y % 16), true);
			if (i == radius  && (j == radius))
			{
				chunks[i][j]->InitNavigation();
				for (int ii = 0; ii < 145; ii++)
				{
					//chunks[i][j]->GetVertices()[ii].color = Utils::Graphics::Color(1, 1, 1, 1);
					for (auto &doodad : chunks[i][j]->GetDoodads())
					{
						for (int ii = 0; ii < doodad.GetVertexCount(); ii++)
						{
						//	doodad.GetVertices()[ii].color = Utils::Graphics::Color(1, 1, 1, 1);
						}
					}
				}
			}
			
		}

	}
	InitBoundingBox();


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
	bounding_box.down.x = -99999.0f;
	bounding_box.down.y = -99999.0f;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
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
					if (!exist)
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
