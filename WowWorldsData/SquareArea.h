#pragma once
#include "Utils.h"
#include "ADTWorker.h"
#include "MapArea.h"
#include "WowInteraction\WowObject.h"
#include "WowObjectAvatar.h"
#include "WMO.h"
using namespace Utils;
using namespace Utils::Game;

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
class Area
{
protected:

	vector<unique_ptr<Doodad>> doodads;
	vector<unique_ptr<WMO>> wmos;

	Chunk *** chunks;
	int radius;
	int area_size;
	Location * location;
	Point2D<int> block_coordinates;
	Point2D<int> coordinates;
	vector<WowObjectAvatar*> wow_object_avatars;

	Utils::Graphics::BoundingBox bounding_box;
	bool IsMoved(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates);

	void CheckAndClearOldObjects();


public:
	
	bool to_update;
	bool busy;
	Area();
	Area(Location * location, Point2D<int> block_coordinates,Point2D<int> coordinates,int radius);
	//Area(Area && area);
	~Area(void);
	Area & operator=(Area && right);
	virtual void Fill(Location * location, Point2D<int> block_coordinates,Point2D<int> coordinates);
	virtual void CheckAndMove(Location * location, Point2D<int> block_coordinates,Point2D<int> coordinates);
	int GetRadius() {return radius;}
	inline int GetSize() { return area_size; }
	Chunk *** GetChunks() { return chunks; }
	vector<WowObjectAvatar*> * GetWowAvatars() {return &wow_object_avatars;}
	void AddWowObjectAvatar(Wow::WowObject* object);
	Utils::Graphics::BoundingBox & GetBoundingBox() { return bounding_box; }
	inline vector<unique_ptr<WMO>> & GetWMOs() { return wmos; }
	inline vector<unique_ptr<Doodad>> & GetDoodads() { return doodads; }
	void DeleteDuplicates();
	void InitAreaBoundingBox();
	void ToMesh();

	
	

};
 
