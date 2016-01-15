#pragma once
#include "Utils.h"
#include "ADTWorker.h"
#include "MapArea.h"
#include "WowInteraction\WowObject.h"
#include "WowObjectAvatar.h"
#include "WMO.h"
using namespace Utils;
using namespace Utils::Game;
struct NavConfig
{
	int m_maxTiles;
	int m_maxPolysPerTile;
	float m_tileSize;
	float m_cellSize;
	float m_cellHeight;
	float m_agentHeight;
	float m_agentRadius;
	float m_agentMaxClimb;
	float m_agentMaxSlope;
	float m_regionMinSize;
	float m_regionMergeSize;
	float m_edgeMaxLen;
	float m_edgeMaxError;
	float m_vertsPerPoly;
	float m_detailSampleDist;
	float m_detailSampleMaxError;
	int m_partitionType;
};
//struct ChunkInfo
//{
//	Location * location;
//	Point2D<int> block_coordinates;
//	Point2D<int> coordinates;
//	Vector3 position;
//	ChunkInfo();
//	
//	ChunkInfo(	Location * location, Point2D<int> block_coordinates,Point2D<int> coordinates,int radius);//:location(location),block_coordinates(block_coordinates),coordinates(coordinates){}
//};
class SquareArea
{
	Chunk *** chunks;
	int radius;
	int area_size;
	Location * location;
	Point2D<int> block_coordinates;
	Point2D<int> coordinates;
	//vector<Doodad*> doodads;
	//vector<Doodad*> old_doodads;
	//vector<Doodad*> active_doodads;
	vector<WowObjectAvatar*> wow_object_avatars;
	//vector<WMO*> wmos;
	//vector<WMO*> old_wmos;
	//vector<WMO*> active_wmos;

	Utils::Graphics::BoundingBox bounding_box;
	//void InitWMOs();
	//void InitDoodads();

public:
	bool to_update;
	bool busy;
	SquareArea();
	SquareArea(Location * location, Point2D<int> block_coordinates,Point2D<int> coordinates,int radius);
	~SquareArea(void);
	SquareArea & operator=(SquareArea && right);
	void Fill(Location * location, Point2D<int> block_coordinates,Point2D<int> coordinates);
	void CheckAndMove(Location * location, Point2D<int> block_coordinates,Point2D<int> coordinates);
	int GetRadius() {return radius;}
	inline int GetSize() { return area_size; }
	Chunk *** GetChunks() { return chunks; }
	//void InitObjects();
	//vector<Doodad*> * GetDoodads() {return &doodads;}
	//vector<Doodad*> * GetActiveDoodas() {return &active_doodads;}
	//const vector<WMO*> & GetWMOs(){ return wmos; }
	//vector<WMO*> & GetActiveWMOs() { return active_wmos; }
	vector<WowObjectAvatar*> * GetWowAvatars() {return &wow_object_avatars;}
	void AddWowObjectAvatar(Wow::WowObject* object);
	//void InitActiveObjects();
	//void ToMesh();
	//void InitBoundingBox();
	Utils::Graphics::BoundingBox & GetBoundingBox() { return bounding_box; }


	
	static NavConfig config;
	vector<unique_ptr<rcPolyMesh>> polys;
	dtNavMeshQuery* m_navQuery;
	dtNavMesh * m_navMesh;
	unique_ptr<PATHDATA>  m_PathStore;
	const static int MAX_SMOOTH = 2048;
	float m_smoothPath[MAX_SMOOTH * 3];
	int m_nsmoothPath;
	void DeleteDuplicates();
	void InitNavigation();
	void InitAreaBoundingBox();
	void BuildAllTiles();
	unsigned char * BuildTileMesh(int x, int y, const float* bmin, const float* bmax, int dataSize);
	int  FindPath(Vector3 & start,Vector3  & end, int nPathSlot);
	static void InitNavConfig();
	void ToMesh();
	static bool getSteerTarget(dtNavMeshQuery* navQuery, const float* startPos, const float* endPos,
		const float minTargetDist,
		const dtPolyRef* path, const int pathSize,
		float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef,
		float* outPoints = 0, int* outPointCount = 0);
	inline static  bool SquareArea::inRange(const float* v1, const float* v2, const float r, const float h);
	static int fixupCorridor(dtPolyRef* path, const int npath, const int maxPath,
		const dtPolyRef* visited, const int nvisited);
	static int SquareArea::fixupShortcuts(dtPolyRef* path, int npath, dtNavMeshQuery* navQuery);

};
 
