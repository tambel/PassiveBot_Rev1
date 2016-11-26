#pragma once
#include "Utils.h"
#include "ADTWorker.h"
#include "MapArea.h"
#include "WMO.h"
#include "Doodad.h"
#include "Chunk.h"


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

class EmptyAreaException : exception
{
public:
	virtual const char * what() const
	{
		return "Can not init area. It is empty";
	}
};

class Area
{
protected:
#ifdef BOTDEV
	float rx;
	float ry;
	float rz;
#endif
	vector<unique_ptr<Doodad>> doodads;
	vector<unique_ptr<WMO>> wmos;
	vector<unique_ptr<Chunk>>chunks;
	int radius;
	int area_size;
	Location  location;
	Point2D<int> block_coordinates;
	Point2D<int> coordinates;
	Utils::Graphics::BoundingBox bounding_box;
	virtual void Update(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates);
	void UpdateCetralizedChunkScale(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates);
	void UpdateCetralizedBlockScale(Location & location, Point2D<int> block_coordinates);
	void _move(Area & other);
	void AddChunk(Location & location, Point2D<int> & block_coordinates, Point2D<int> & coordinates);
	void InitAreaBoundingBox();
public:
	
	Area();
	Area(int radius);
	Area(Location & location, Point2D<int> block_coordinates,Point2D<int> coordinates,int radius);
	
	virtual ~Area(void);
	Area & operator=(Area && right);
	Area(Area && area);
	
	void CheckAndUpdate(Location & location, Point2D<int> block_coordinates,Point2D<int> coordinates);
	inline void UpdateArea(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates) { this->Update(location, block_coordinates, coordinates); }
	//void Update(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates);
	inline int GetRadius() {return radius;}
	inline int GetSize() { return area_size; }
	inline Utils::Graphics::BoundingBox & GetBoundingBox() { return bounding_box; }
	inline vector<unique_ptr<WMO>> & GetWMOs() { return wmos; }
	inline vector<unique_ptr<Doodad>> & GetDoodads() { return doodads; }
	inline Location & GetLocation() { return location; }
	 vector<unique_ptr<Chunk>> & GetChunks() { return chunks; }
	bool IsOutOfBounds(Location & location, Point2D<int> block_coordinates, Point2D<int> coordinates);
	void InitMapObjects();


#ifdef BOTDEV

	void ToMesh();
	void Rotate();
#endif

	//virtual void CheckAndMoveImpl(Location * location, Point2D<int> block_coordinates, Point2D<int> coordinates);
	
	

};
 
