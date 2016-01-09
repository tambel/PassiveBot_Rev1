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


};

