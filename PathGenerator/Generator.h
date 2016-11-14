#pragma once
#include "WowWorldsData\WowWorldsData.h" 
#include "GlobalNavMesh.h"





struct pt
{
	Vector3 * path;
	unsigned size;
};

class Generator
{
	NavArea * area;
	static vector<int> chunk_vectors_order;
	static void init_order();
	static float CalcPathDistance(Vector3 * path, unsigned size);
	static float CalcDistance(Vector3 & point1, Vector3 point2);
	static bool IsPointsConnected(Path::Point * point1, Path::Point * point2);
	void ConnectPoints(Path::Point * point1, Path::Point * point2, Vector3 * path, unsigned path_size);
	vector<unique_ptr<Path::Chunk>> chunks;
	vector<unique_ptr<Path::Link>> links;
	vector<unique_ptr<Path::Point>> points;

	/////

	GlobalNavMesh nm;
	
public:
	vector<pt> paths;


	Generator();
	~Generator();
	void LinkChunkWithNeighbours(Point2D<int> block, Point2D<int> coordinates);
	vector<Path::Point*> * GetChunkPoints(Point2DI & block,Point2DI & coordinates);
	Path::Chunk * GetChunk(Point2DI & block, Point2DI & coordinates);
	
	void LinkTwoChunk(Chunk * c1,Chunk * c2, NavArea & area);
	vector<unique_ptr<Path::Link>> & GetLinks();


	/////////////////////////


	void CreateContinentNavMesh();
	void AddBlockTiles();

};

