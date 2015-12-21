#include "stdafx.h"
#include <algorithm>
SquareArea::SquareArea(Location * location, Point2D<int> block_coordinates,Point2D<int> coordinates,int radius):location(location), block_coordinates(block_coordinates),coordinates(coordinates),radius(radius)
{
	area_size=radius*2+1;
	chunks=new Chunk**[area_size];
	doodads=vector<Doodad*>();
	to_update=true;
	old_doodads=vector<Doodad*>();
	active_doodads=vector<Doodad*>();
	wow_object_avatars=vector<WowObjectAvatar*>();
	vector<Model*> additional_objects;
	for (int i=0;i<area_size;i++)
	{
		chunks[i]=new Chunk * [area_size];
		for (int j=0;j<area_size;j++)
		{
			chunks[i][j]=0;
		}
	}
	Fill(location,block_coordinates,coordinates);
	InitObjects();
	/*
	Chunk *** tmp_chunks;
	int area_size=radius*2+1;
	tmp_chunks=new Chunk**[area_size];
	//chunks=new Chunk * [area_size*area_size];
	//chunks=vector<Chunk*>();
	for (int i=0;i<area_size;i++)
	{
	tmp_chunks[i]=new Chunk * [area_size];
	chunks[i]=new Chunk * [area_size];
	}
	for (int i=0;i<area_size;i++)
	{
	for (int j=0;j<area_size;j++)
	{
	chunks[i][j]=0;
	tmp_chunks[i][j]=0;
	}
	}
	//Point2D<int> abs_center_pos=Point2D<int>(coordinates+Point2D<int>(16,16));
	//Point2D<int> abs_center_pos=coordinates+Point2D<int>(16,16);
	Point2D<int> area_position=(coordinates+Point2D<int>(16,16))-Point2D<int>(radius,radius);
	unsigned counter=0;
	for (int i=0;i<area_size;i++)
	{
	for (int j=0;j<area_size;j++)
	{
	Point2D<int> abs_pos=area_position+Point2D<int>(i,j);
	tmp_chunks[i][j]=ADTWorker::GetChunk(location,block_coordinates+=(Point2D<int>(abs_pos.X/16,abs_pos.Y/16)-Point2D<int>(1,1)),Point2D<int>(abs_pos.X%16,abs_pos.Y%16));

	//chunks[chunk_number]=tmp_chunks[i][j];
	if (tmp_chunks[i][j])
	{
	tmp_chunks[i][j]->SetRelativePosition(Vector3(j*Metrics::ChunkSize,-i*Metrics::ChunkSize,tmp_chunks[i][j]->GetGamePosition().z));
	//chunks.push_back(tmp_chunks[i][j]);

	}
	if (i==0)
	{
	for (int v=0;v<145;v++)
	{
	if (j<10)
	tmp_chunks[i][j]->GetVertices()[v].color=Utils::Graphics::Color(1.0f,0.0f,1.0f,0.0f);
	if (j>10)
	tmp_chunks[i][j]->GetVertices()[v].color=Utils::Graphics::Color(0.0f,1.0f,1.0f,0.0f);
	}
	}
	counter++;
	}
	}
	*/
}


SquareArea::~SquareArea(void)
{
}

void SquareArea::Fill(Location * location, Point2D<int> block_coordinates,Point2D<int> coordinates)
{
	this->location=location;
	this->block_coordinates=block_coordinates;
	this->coordinates=coordinates;
	int area_size=radius*2+1;
	Point2D<int> area_position=(coordinates+Point2D<int>(16,16))-Point2D<int>(radius,radius);
	Vector3 block_real_position=Vector3(block_coordinates.Y*Metrics::BlockSize,-block_coordinates.X*Metrics::BlockSize,0.0f);
	for (int i=0;i<area_size;i++)
	{
		for (int j=0;j<area_size;j++)
		{
			Point2D<int> abs_pos=area_position+Point2D<int>(i,j);

			chunks[i][j]=ADTWorker::GetChunk(location,block_coordinates+=(Point2D<int>(abs_pos.X/16,abs_pos.Y/16)-Point2D<int>(1,1)),Point2D<int>(abs_pos.X%16,abs_pos.Y%16),true);
			if (chunks[i][j])
			{
				//chunks[i][j]->SetGamePosition(Vector3(-chunks[i][j]->GetGamePosition().y,chunks[i][j]->GetGamePosition().x,chunks[i][j]->GetGamePosition().z));
				/*
				if (i==0)
				{
				for (int v=0;v<145;v++)
				{
				if (j<10)
				chunks[i][j]->GetVertices()[v].color=Utils::Graphics::Color(1.0f,0.0f,1.0f,0.0f);
				if (j>10)
				chunks[i][j]->GetVertices()[v].color=Utils::Graphics::Color(0.0f,1.0f,1.0f,0.0f);

				}
				}

				if (chunks[i][j]->GetCoordinates()==this->coordinates && chunks[i][j]->GetBlockCoordinates()==this->block_coordinates && chunks[i][j]->GetLocation()->id==this->location->id)
				{
				for (int v=0;v<145;v++)
				{
				chunks[i][j]->GetVertices()[v].color=Utils::Graphics::Color(1.0f,1.0f,1.0f,1.0f);

				}
				}
				*/
			}
		}

	}


}

void SquareArea::Move(Location * location, Point2D<int> block_coordinates,Point2D<int> coordinates)
{

	if (this->location->id != location->id || this->block_coordinates != block_coordinates || this->coordinates != coordinates)
	{
		if (!busy)
		{
			busy=true;
			bool block_changed=false;
			if (this->location->id != location->id || this->block_coordinates != block_coordinates)
				block_changed=true;
			Fill(location,block_coordinates,coordinates);
			if (block_changed)
				InitObjects();
			InitActiveDoodads();
			to_update=true;
			busy=false;
		}

	}

}
void SquareArea::InitObjects()
{
	doodads.clear();
	for (int i=block_coordinates.X-1;i<=block_coordinates.X+1;i++)
	{
		for (int j=block_coordinates.Y-1;j<=block_coordinates.Y+1;j++)
		{
			ADT * adt=ADTWorker::GetADT(location,Point2D<int>(i,j));
			if (!adt)
				continue;
			for (auto m2_info:*adt->GetM2Infos())
			{
				bool exist=false;
				bool duplicate=false;
				for (auto &old_doodad:old_doodads)
				{
					if (old_doodad)
					{
						if (old_doodad->GetUUID()==m2_info.mddf.UniqueId)
						{
							exist=true;
							doodads.push_back(old_doodad);
							old_doodad=0;

							break;
						}
					}
				}
				if (exist )
				{
					continue;
				}

				M2 * m2= new M2(m2_info.file);
				Doodad * doodad=new Doodad(m2,m2_info.mddf.UniqueId,Position(m2_info.mddf.Position,m2_info.mddf.Rotation),m2_info.mddf.Scale);
				unsigned long d=doodad->GetUUID();
				doodads.push_back(doodad);
				delete m2;
			}
		}
	}
	int count=0;
	for (auto doodad:doodads)
	{
		for (auto &doodad2:doodads)
		{
			if (doodad && doodad2)
			{
				if (doodad->GetUUID()==doodad2->GetUUID() && doodad!=doodad2)
				{
					count++;
					doodad2=0;
				}
			}
		}
	}
	doodads.erase(remove(doodads.begin(), doodads.end(), (Doodad*)0), doodads.end());
	for (auto old_doodad:old_doodads)
	{
		delete old_doodad;
	}
	old_doodads.clear();
	for (auto doodad:doodads)
	{
		old_doodads.push_back(doodad);
	}


}
void SquareArea::AddWowObjectAvatar(Wow::WowObject * object)
{
	WowObjectAvatar * avatar=new WowObjectAvatar(object);
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
void SquareArea::InitActiveDoodads()
{

	bounding_box.up.x=99999.0f;
	bounding_box.up.y=99999.0f;
	for (int i=0;i<area_size;i++)
	{
		for (int j=0;j<area_size;j++)
		{
			if (bounding_box.up.x>chunks[i][j]->GetRealPosition().x)
			{
				bounding_box.up.x=chunks[i][j]->GetRealPosition().x;
			}
			if (bounding_box.up.y>chunks[i][j]->GetRealPosition().y)
			{

				bounding_box.up.y=chunks[i][j]->GetRealPosition().y;
			}
		}
	}
	bounding_box.down=bounding_box.up+Vector3(area_size*Metrics::ChunkSize,area_size*Metrics::ChunkSize,0);
	active_doodads.clear();
	for (auto doodad:doodads)
	{
		if (bounding_box.IsInside2D(doodad->GetPosition().coords))
		{
			active_doodads.push_back(doodad);
		}
	}
}
