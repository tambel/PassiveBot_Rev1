#pragma once
#include "Utils.h"
#include "ADTWorker.h"
#include "MapArea.h"
#include "WowInteraction\WowObject.h"
#include "WowObjectAvatar.h"
#include "WMO.h"
using namespace Utils;
using namespace Utils::Game;
struct ChunkInfo
{
	Location * location;
	Point2D<int> block_coordinates;
	Point2D<int> coordinates;
	Vector3 position;
	ChunkInfo();
	
	ChunkInfo(	Location * location, Point2D<int> block_coordinates,Point2D<int> coordinates,int radius);//:location(location),block_coordinates(block_coordinates),coordinates(coordinates){}
};
class SquareArea
{

	Navigation navigation;
	Chunk *** chunks;
	int radius;
	int area_size;
	Location * location;
	Point2D<int> block_coordinates;
	Point2D<int> coordinates;
	vector<Doodad*> doodads;
	vector<Doodad*> old_doodads;
	vector<Doodad*> active_doodads;
	vector<WowObjectAvatar*> wow_object_avatars;
	vector<WMO*> wmos;
	vector<WMO*> old_wmos;
	vector<WMO*> active_wmos;

	Utils::Graphics::BoundingBox bounding_box;
	void InitWMOs();
	void InitDoodads();
	bool nav_mesh_ready;

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
	void InitObjects();
	vector<Doodad*> * GetDoodads() {return &doodads;}
	vector<Doodad*> * GetActiveDoodas() {return &active_doodads;}
	const vector<WMO*> & GetWMOs(){ return wmos; }
	vector<WMO*> & GetActiveWMOs() { return active_wmos; }
	vector<WowObjectAvatar*> * GetWowAvatars() {return &wow_object_avatars;}
	void AddWowObjectAvatar(Wow::WowObject* object);
	void InitActiveObjects();
	void ToMesh();
	void InitBoundingBox();
	Utils::Graphics::BoundingBox & GetBoundingBox() { return bounding_box; }
	Navigation & Navigation() {	return navigation;}


	int m_maxTiles = 1024;
	int m_maxPolysPerTile = 4096;
	float m_tileSize = 32.0;
	float m_cellSize = 0.3;
	float m_cellHeight = 0.2;
	float m_agentHeight = 2.0;
	float m_agentRadius = 1.0f; //0.600000024;
	float m_agentMaxClimb = 0.899999976;
	float m_agentMaxSlope = 45.0000000;
	float m_regionMinSize = 8.00000000;
	float m_regionMergeSize = 20.0000000;
	float m_edgeMaxLen = 12.0000000;
	float m_edgeMaxError = 1.29999995;
	float m_vertsPerPoly = 6.00000000;
	float m_detailSampleDist = 6.00000000;
	float m_detailSampleMaxError = 1.00000000;
	int m_partitionType = 0;

	vector<rcPolyMesh*> polys;
	dtNavMeshQuery* m_navQuery;
	dtNavMesh * m_navMesh;
	void TestNav();
	PATHDATA  m_PathStore;
	void InitNavigation();
	void DeleteDuplicates();
	void InitNavigation2();
	void InitChunkyMesh();
	void InitNavigationR();
	void InitAreaBoundingBox();
	void BuildAllTiles();
	unsigned char * BuildTileMesh(int x, int y, const float* bmin, const float* bmax, int dataSize);
	int  FindPath(Vector3 & start,Vector3  & end, int nPathSlot);


};

