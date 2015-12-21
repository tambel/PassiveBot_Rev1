#pragma once
#include "Utils.h"
#include "ChunkedData.h"
#include "Chunk.h"
#include "ADTStructs.h"
//#include <string>
using namespace Utils;
using namespace std;
using namespace Game;
struct M2Info
{
	string file;
	MDDF mddf;
	M2Info(string file,MDDF mddf):file(file),mddf(mddf){}
};


class ADT:public ChunkedData
{
private:
	string path;
	Location * location;
	Point2D<int> coordinates;
	BinaryReader * root_reader;
	//BinaryReader * tex;
	BinaryReader * obj_reader;
	ChunkStreamInfo chunk_stream_infos[16][16];
	vector<M2Info> m2_infos;
	void ReadM2Models(bool hight_detalization=false);
public:
	ADT(Location * location,  Point2D<int> coordinates);
	~ADT(void);
	Chunk * GetChunk(Point2D<int> coordinates);
	Location * GetLocation() {return location;}
	Point2D<int> GetCoordinates(){return coordinates;}
	bool IsExist() {return root_reader->IsFileExist();}
	bool operator==(const ADT & right);
	void ReadObjects(bool hight_detalization=false);
	vector<M2Info> * GetM2Infos() {return &m2_infos;}


};


/*
struct MVER {
unsigned  version;
};
struct MCVT 
{
float height[145];
};
struct MCNK
{
unsigned offset;
Vector3 position;
MCVT mcvt;
Point2D<int> coords;
};
struct MWMO
{
unsigned long length;
//string * names;
char * names;
};
struct MWID
{
unsigned long length;
unsigned long * offsets;
};
struct MMDX 
{
unsigned long length;
//string * names;
char * names;
};
struct MMID
{
unsigned long length;
unsigned long * offsets;
};
struct MODF
{
unsigned mwidEntry;           // references an entry in the MWID chunk, specifying the model to use.
unsigned uniqueId;            // this ID should be unique for all ADTs currently loaded. Best, they are unique for the whole map.
Vector3 position;
Vector3 rotation;            // same as in MDDF.
float lowerBounds[3];         // these two are position plus the wmo bounding box.
float upperBounds[3];
unsigned wtf[2];
};
struct MDDF
{
unsigned mwidEntry;           // references an entry in the MWID chunk, specifying the model to use.
unsigned uniqueId;            // this ID should be unique for all ADTs currently loaded. Best, they are unique for the whole map.
Vector3 position;
Vector3 rotation;     
unsigned short scale;

unsigned short wtf;
};
class Adt
{
bool is_file_exists;
char * root;
unsigned long root_length;
char * obj;
unsigned long obj_length;
char * tex;
unsigned long tex_length;
public:

MVER mver;
vector<MCNK*> mcnk_list;
/*
MWMO * mwmo;
MWID * mwid;
MODF * modf;

vector<WmoInfo> wmo_infos;
vector <M2Info> m2_infos;
Adt(string  path);
~Adt(void);
unsigned GetVersion();
bool IsExist();

};

*/