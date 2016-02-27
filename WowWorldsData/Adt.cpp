#include "stdafx.h"
using namespace std;
using namespace ChunkSignatures;

ADT::ADT(Location & location,Point2D<int> coordinates)
{
	this->location=location;
	this->coordinates=coordinates;
	string terrian_path="World\\Maps\\";
	path=Configuration::GetGameDataPath()+terrian_path+location.name+"\\"+location.name+"_"+std::to_string(coordinates.Y)+"_"+std::to_string(coordinates.X);
	root_reader=new BinaryReader(path+".adt");
	if (!root_reader->IsFileExist())
		throw exception("Adt file not exist");
	for (int i=0;i<256;i++)
	{
		SeekChunk(root_reader,ChunkSignatures::ADTSignature::Mcnk);
		unsigned chunk_size = root_reader->ReadUInt();
		chunk_stream_infos[i/16][i%16]=ChunkStreamInfo(root_reader->GetPosition(),chunk_size,root_reader);
		root_reader->MoveForward(chunk_stream_infos[i/16][i%16].size);
	}
	ReadObjects();
}
ADT::~ADT()
{
	delete[] wmos_filenames;
	wmos_filenames = nullptr;
	delete[]doodads_filenames;
	doodads_filenames = nullptr;
	delete[] wmos_ids;
	wmos_ids = nullptr;
	delete[] doodads_ids;
	doodads_ids = nullptr;

	delete[] modfs;
	delete[] mddfs;
	modfs = nullptr;
	mddfs = nullptr;

	delete root_reader;
	root_reader=0;

	delete obj_reader;
	obj_reader = nullptr;
	m2_infos.clear();
	wmo_infos.clear();
}
Chunk * ADT::GetChunk(Area * area,Point2D<int> coordinates)
{
	return new Chunk(area,chunk_stream_infos[coordinates.X][coordinates.Y], obj_chunk_stream_infos[coordinates.X][coordinates.Y],this,coordinates);
}

bool ADT::operator==(const ADT & right)
{
	return this->location==right.location && this->coordinates==right.coordinates && this->location==right.location;
}
void ADT::ReadObjects(bool hight_detalization)
{

	obj_reader=new BinaryReader(path+"_obj"+to_string((int)hight_detalization)+".adt");
	if (!obj_reader->IsFileExist()) return;

	for (int i = 0; i<256; i++)
	{
		SeekChunk(obj_reader, ChunkSignatures::ADTSignature::Mcnk);
		unsigned chunk_size = obj_reader->ReadUInt();
		obj_chunk_stream_infos[i / 16][i % 16] = ChunkStreamInfo(obj_reader->GetPosition(), chunk_size, obj_reader);
		obj_reader->MoveForward(obj_chunk_stream_infos[i / 16][i % 16].size);
	}
	obj_reader->MoveToBegining();
	ReadM2Models(hight_detalization);
	ReadWMOModels();

}
void ADT::ReadM2Models(bool hight_detalization)
{
	m2_infos=vector<M2Info>();
	
	unsigned doodads_filenames_length=0;
	
	unsigned doodads_ids_length=0;
	mddfs;
	unsigned mddf_count=0;
	if (!ChunkedData::SeekChunk(obj_reader,Utils::ChunkSignatures::ADTSignature::Mmdx,true))
	{
		doodads_filenames_length=obj_reader->Read<unsigned>(); 
		doodads_filenames=new char[doodads_filenames_length];
		obj_reader->ReadArray<char>(doodads_filenames,doodads_filenames_length);
	}
	if (!ChunkedData::SeekChunk(obj_reader,Utils::ChunkSignatures::ADTSignature::Mmid,true))
	{
		doodads_ids_length=obj_reader->Read<unsigned>()/4; 
		doodads_ids=new unsigned[doodads_ids_length];
		
		
		obj_reader->ReadArray<unsigned>(doodads_ids,doodads_ids_length);
	}
	if (!ChunkedData::SeekChunk(obj_reader,Utils::ChunkSignatures::ADTSignature::Mddf,true))
	{
		unsigned size=obj_reader->Read<unsigned>();
		mddf_count=size/sizeof(MDDF);
		mddfs=new MDDF[mddf_count];
		obj_reader->ReadArray<MDDF>(mddfs,mddf_count); 
	}
	for (unsigned i=0;i<mddf_count;i++)
	{
		m2_infos.push_back(M2Info(Configuration::GetGameDataPath()+(doodads_filenames+doodads_ids[mddfs[i].Mmid]),mddfs[i]));
		//M2 * m2= new M2(Configuration::GetGameDataPath()+(doodads_filenames+doodads_ids[mddfs[i].Mmid]));
		//Doodad * doodad = new Doodad(m2,mddfs[i].UniqueId,mddfs[i].Position,mddfs[i].Rotation,mddfs[i].Scale);
	}
	//delete [] doodads_filenames;
	//delete [] doodads_ids;
	//delete [] mddfs;
}
void ADT::ReadWMOModels()
{
	obj_reader->MoveToBegining();
	wmo_infos = vector<WMOInfo>();
	unsigned wmos_filenames_length = 0;
	unsigned wmos_ids_length = 0;
	unsigned modf_count = 0;
	if (!ChunkedData::SeekChunk(obj_reader, Utils::ChunkSignatures::ADTSignature::Mwmo, true))
	{
		wmos_filenames_length = obj_reader->Read<unsigned>();
		wmos_filenames = new char[wmos_filenames_length];

		obj_reader->ReadArray<char>(wmos_filenames, wmos_filenames_length);
	}
	if (!ChunkedData::SeekChunk(obj_reader, Utils::ChunkSignatures::ADTSignature::Mwid, true))
	{
		wmos_ids_length = obj_reader->Read<unsigned>() / 4;
		wmos_ids = new unsigned[wmos_ids_length];


		obj_reader->ReadArray<unsigned>(wmos_ids, wmos_ids_length);
	}
	if (!ChunkedData::SeekChunk(obj_reader, Utils::ChunkSignatures::ADTSignature::Modf, true))
	{
		unsigned size = obj_reader->Read<unsigned>();
		modf_count = size / sizeof(MODF);
		modfs = new MODF[modf_count];
		obj_reader->ReadArray<MODF>(modfs, modf_count);
	}
	for (unsigned i = 0; i<modf_count; i++)
	{
		/*char ss[1000];
		strcpy(ss, wmos_filenames + wmos_ids[modfs[i].Mwid]);
		
		string s = ss;
		string n = Configuration::GetGameDataPath() + s;
		WMOInfo info =WMOInfo(n, modfs[i]);
		wmo_infos.push_back(info);*/
		wmo_infos.push_back(WMOInfo(Configuration::GetGameDataPath() + (wmos_filenames + wmos_ids[modfs[i].Mwid]), modfs[i]));
	}
	//delete [] wmos_filenames;
	//delete[] wmos_ids;
	//delete[] modfs;
}
