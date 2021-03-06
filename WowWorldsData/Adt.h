#pragma once
#include "Utils.h"
#include "Chunk.h"
#include "ChunkedData.h"
#include "ADTStructs.h"
#include "WMORoot.h"
#include "Area.h"
//#include <string>



class Chunk;

class ADT:public ChunkedData
{
private:
	string path;
	Location location;
	Point2D<int> coordinates;
	BinaryReader * root_reader;
	BinaryReader * obj_reader;
	ChunkStreamInfo chunk_stream_infos[16][16];
	ChunkStreamInfo obj_chunk_stream_infos[16][16];
	vector<M2Info> m2_infos;
	char * doodads_filenames=nullptr;
	unsigned * doodads_ids = nullptr;
	MDDF * mddfs = nullptr;
	MODF * modfs = nullptr;
	char * wmos_filenames = nullptr;
	unsigned * wmos_ids = nullptr;
	vector<WMOInfo> wmo_infos;
	void ReadM2Models(bool hight_detalization=false);
	void ReadWMOModels();
	vector<string> wmofilenames;
public:
	ADT(Location & location,  Point2D<int> coordinates);
	~ADT(void);
	Chunk * GetChunk(Area * area,Point2D<int> coordinates);
	Location & GetLocation() {return location;}
	Point2D<int> GetCoordinates(){return coordinates;}
	bool IsExist() {return root_reader->IsFileExist();}
	bool operator==(const ADT & right);
	void ReadObjects(bool hight_detalization=false);
	vector<M2Info> * GetM2Infos() {return &m2_infos;}
	const vector<WMOInfo> & GetWMOInfos() { return wmo_infos; }
	const char * GetDoodadsFilenames() { return doodads_filenames; }
	const unsigned * GetDoodadsIds() { return doodads_ids; }
	const MDDF * GetMDDFs() { return mddfs; }
	const char * GetWMOFilenames() { return wmos_filenames; }
	const unsigned * GetWMOsIds() { return wmos_ids; }
	const MODF * GetMODFs() { return modfs; }
};

