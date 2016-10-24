#include "stdafx.h"
#include <Windows.h>

int * ChunkModel::indices=ChunkModel::Init();

Chunk::Chunk(void)
{
}
Chunk::Chunk(Area * area,ChunkStreamInfo info, ChunkStreamInfo obj_info,  ADT * adt_file/*, Location * location, Point2D<int> block_coordinates*/,Point2D<int> coordinates):
	Model(),
	MapObject(),
	root_info(info),
	adt(adt_file)/*,location(location),block_coordinates(block_coordinates)*/,
	coordinates(coordinates),
	area(area)
{
	//adt_ptr = shared_ptr<ADT>(adt);
	yey=false;
	result_mesh = 0;
	location = adt->GetLocation();
	block_coordinates = adt->GetCoordinates();
	//wmos = vector<WMO*>();
	header=MCNK();
	root_info=info;
	root_reader=root_info.reader;
	this->obj_reader = obj_info.reader;
	//indices=ChunkModel::indices;
	index_count=ChunkModel::index_count;
	indices = new int[768];
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
	int counter2 = 0;
	//vertices=new Utils::Graphics::Vertex[145];
	vertices = new float[435];
	for(int i = 0; i < 17; ++i)
	{
		for(int j = 0; j < (((i % 2) != 0) ? 8 : 9); ++j)
		{
			float height = position.coords.z + heights[counter];
			float x = position.coords.x + j * Metrics::UnitSize;
			if ((i % 2) != 0)
				x += 0.5f * Metrics::UnitSize;
			float y = position.coords.y - i * Metrics::UnitSize * 0.5f;

			//vertices[counter].position =Vector3(x, y, heights[counter]);
			vertices[counter2++] = x + real_position.x;
			vertices[counter2++] = heights[counter] + real_position.z;
			vertices[counter2++] = y + real_position.y;
			++counter;
		}
	}
	for (unsigned i = 0; i < index_count; i += 3)
	{
		indices[i] = ChunkModel::indices[i+2];
		indices[i+1] = ChunkModel::indices[i + 1];
		indices[i+2] = ChunkModel::indices[i];
	}
	rcCalcBounds(vertices, vertex_count, reinterpret_cast<float*>(&bounding_box.up), reinterpret_cast<float*>(&bounding_box.down));

	delete [] heights;
}
void Chunk::LoadMcrd(unsigned long size)
{
	unsigned count = size / 4;
	unique_ptr<unsigned> doodads_refs = unique_ptr<unsigned>(new unsigned[count]);
	obj_reader->ReadArray<unsigned>(doodads_refs.get(), count);
	for (unsigned i = 0; i < count; i++)
	{
		
		MDDF mddf = adt->GetMDDFs()[doodads_refs.get()[i]];
		M2Info info = M2Info(/*Configuration::GetGameDataPath() +*/(adt->GetDoodadsFilenames() + adt->GetDoodadsIds()[mddf.Mmid]),mddf);
		doodad_infos.push_back(info);
	//	doodad_uuids.push_back(mddf.UniqueId);
	//	mddfs.push_back(mddf);
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
		WMOInfo info = WMOInfo(Configuration::GetGameDataPath() + (adt->GetWMOFilenames() + adt->GetWMOsIds()[modf.Mwid]), modf);
		wmo_infos.push_back(info);
		//wmo_uuids.push_back(modf.UniqueId);
		//modfs.push_back(modf);
	}
}
bool Chunk::operator==(const Chunk & right)
{
	return this->location==right.location && this->block_coordinates==right.block_coordinates && this->coordinates==right.coordinates;
}

Chunk & Chunk::operator=(Chunk && other)
{
	area = other.area;
	return *this;
}
