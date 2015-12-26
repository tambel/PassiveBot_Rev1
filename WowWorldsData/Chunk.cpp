#include "stdafx.h"

unsigned short* ChunkModel::indices=ChunkModel::Init();

Chunk::Chunk(void)
{
}
Chunk::Chunk(ChunkStreamInfo info, Location * location, Point2D<int> block_coordinates,Point2D<int> coordinates):root_info(info),location(location),block_coordinates(block_coordinates),coordinates(coordinates)
{
	header=MCNK();
	root_info=info;
	root_reader=root_info.reader;
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
	while (root_reader->GetPosition()<=root_info.start+root_info.size)
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
			if (header.IndexX==0 ||  header.IndexY==0 ||  header.IndexX==15 ||  header.IndexY==15)
				{
					
					vertices[counter].color=Graphics::Color(0.0f,1.0f,1.0f,0.0f);
				}
			++counter;
		}
	}

	delete [] heights;
}
bool Chunk::operator==(const Chunk & right)
{
	return this->location->id==right.location->id && this->block_coordinates==right.block_coordinates && this->coordinates==right.coordinates;
}
void Chunk::SearchForObjects()
{

}