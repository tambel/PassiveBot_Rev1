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
class SquareArea: public MapArea
{
	
	//Chunk *** chunks;
	//Chunk ***chunks;
	Chunk * chunks2[21][21];
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
	vector<WMO*> wmosp;
	vector<WMO*> old_wmosp;
	vector<WMO*> active_wmosp;
	Utils::Graphics::BoundingBox bounding_box;
	void InitWMOs();
	void InitDoodads();

public:
	bool to_update;
	SquareArea(Location * location, Point2D<int> block_coordinates,Point2D<int> coordinates,int radius);
	~SquareArea(void);
	void Fill(Location * location, Point2D<int> block_coordinates,Point2D<int> coordinates);
	void Move(Location * location, Point2D<int> block_coordinates,Point2D<int> coordinates);
	int GetRadius() {return radius;}
	void InitObjects();
	vector<Doodad*> * GetDoodads() {return &doodads;}
	vector<Doodad*> * GetActiveDoodas() {return &active_doodads;}
	const vector<WMO*> & GetWMOs(){ return wmos; }
	vector<WMO*> & GetActiveWMOs() { return active_wmos; }
	vector<WowObjectAvatar*> * GetWowAvatars() {return &wow_object_avatars;}
	void AddWowObjectAvatar(Wow::WowObject* object);
	void InitActiveObjects();
	void ToMesh();
	Model<unsigned> ToSoloModel();
	void InitBoundingBox();
	Utils::Graphics::BoundingBox & GetBoundingBox() { return bounding_box; }


};

