#pragma once
#include "ChunkedData.h"
#include "Utils.h"
#include "Model.h"
#include "Doodad.h"
#include "WMO.h"
#include "ADTStructs.h"
#include "Adt.h"
//#include "Tools\Configuration.h"

#include "Recast.h"

#include "Sample.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "DetourCommon.h"


using namespace Game;
class Area;
struct ChunkAdditionalData
{
	MDDF * mddfs;
	unsigned mddfs_count;
};




class ChunkModel
{
public:
	
	static const unsigned long index_count=768;
	static int * indices;
	static const unsigned long vertex_count=145;
	static int* Init()
	{
		static int *indices=new int[index_count];
		for (unsigned y = 0; y < 8; ++y)
		{
			for (unsigned x = 0; x < 8; ++x)
			{
				unsigned i = y * 8 * 12 + x * 12;
				indices[i + 0] = y * 17 + x;
				indices[i + 2] = y * 17 + x + 1;
				indices[i + 1] = y * 17 + x + 9;

				indices[i + 3] = y * 17 + x + 1;
				indices[i + 5] = y * 17 + x + 18;
				indices[i + 4] = y * 17 + x + 9;

				indices[i + 6] = y * 17 + x + 18;
				indices[i + 8] = y * 17 + x + 17;
				indices[i + 7] = y * 17 + x + 9;

				indices[i + 9] = y * 17 + x + 17;
				indices[i + 11] = y * 17 + x;
				indices[i + 10] = y * 17 + x + 9;
			}
		}
		return indices;
	}
	static void  Clear()
	{
		delete[] indices;
		indices = nullptr;
	}
};
struct M2info;
class ADT;
struct WMOInfo;

class Chunk:public Model,public MapObject
{
private:
	 vector<unsigned> doodad_uuids;
	 vector <unsigned> wmo_uuids;
	 vector<MDDF> mddfs;
	 vector <MODF> modfs;
	 vector<WMOInfo> wmo_infos;
	 vector<M2Info> doodad_infos;

	ADT * adt;
	shared_ptr<ADT> adt_ptr;

	Area * area;
	Location location;
	Point2D<int> block_coordinates;
	Point2D<int> coordinates;
	Vector3 game_position;
	//Vector3 position;
	Vector3 real_position;
	vector<Doodad*> doodads;
	//vector<WMO> wmos;
	//vector<Doodad*> doodads;
	vector<WMO*> wmos;
	BinaryReader * root_reader;
	BinaryReader * obj_reader;
	
	ChunkStreamInfo root_info;
	MCNK header;
	//Utils::Graphics::Vertice * vertices;
	//static unsigned * indices;
	void LoadMcvt();
	void LoadMcrd(unsigned long size);
	void LoadMcrw(unsigned long size);
	Utils::Geometry::BoundingBox terrain_bounding_box;
	Utils::Geometry::BoundingBox full_bounding_box;
	



public:
	unique_ptr<float> nav_vertices;
	unique_ptr<int> nav_indices;
	unsigned long vert_count;
	unsigned long ind_count;;
	void InitNavigation();
	
	dtNavMeshQuery * m_navQuery = 0;;
	float bmin[3];
	float bmax[3];
	rcPolyMesh * result_mesh;
	unsigned char * navData;
	int navDataSize = 0;
	bool yey;



	bool is_active;
	bool is_new;
	Chunk(void);
	~Chunk(void);
	Chunk(Area * area,ChunkStreamInfo info, ChunkStreamInfo obj_info, ADT * adt_file/*, Location * loc,Point2D<int> block_coordinates*/,Point2D<int> coordinates);

	Location & GetLocation() {return location;}
	Point2D<int> GetBlockCoordinates() {return block_coordinates;}
	Point2D<int> GetCoordinates() {return coordinates;}
	//Utils::Graphics::Vertice * GetVertices() {return vertices;}
	bool operator==(const Chunk & right);
	//static unsigned long* GetIndices() {return s_fileds.indices;}
	Vector3 GetGamePosition() {return game_position;}
	void SetGamePosition(Vector3 position) {game_position=position;}
	Vector3 GetRealPosition() {return real_position;}
	void  SetRealPosition(Vector3 pos) {real_position=pos;}
	vector<Doodad*> & GetDoodads() { return doodads; }
	vector<WMO*> & GetWMOs() { return wmos; }
	inline Utils::Geometry::BoundingBox & GetTerrainBoundingBox() { return terrain_bounding_box; }
	inline Utils::Geometry::BoundingBox & GetFullBoundingBox() { return full_bounding_box; }
	inline Area * GetArea() { return area; }
	Chunk & operator=(Chunk &&);
	inline vector<WMOInfo>  & GetWMOInfos() { return wmo_infos; }
	inline vector<M2Info> & GetDoodadInfos() { return doodad_infos; }
	inline shared_ptr<ADT> GetADT() { return adt_ptr; }
	 void SetADTptr(shared_ptr<ADT> & ptr) { adt_ptr = ptr; }
};

