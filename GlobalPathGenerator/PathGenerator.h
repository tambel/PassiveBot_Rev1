#pragma once
#include "WowWorldsData\NavArea.h"
#include "WowInteraction\Utils.h"
#ifdef _WIN32
#include  <Windows.h>
#endif


struct SavedState
{
	unsigned location_id;
	Point2D<int> stopped_block;
	Point2D<int> stopped_chunk;

	Point2D<int> finish_block;
	Point2D<int> finish_chunk;
};

struct MapPoint
{
	Vector3 position;
	unsigned location_id;
	Point2D<int> block;
	Point2D<int> chunk;
};

//struct Path
//{
//	Vector3 * points;
//	unsigned points_number;
//};

struct Path
{
	Vector3 points[500];
	unsigned points_number;
};

struct ChunksLink
{
	MapPoint chunk1;
	MapPoint chunk2;
	Path path;

};

class PathGenerator
{
	NavArea area;
	
	static Point2D<int> ConvertMapMerticsToGlobal(Point2D<int> block, Point2D<int> chunk);
	static void SaveState(Location & location, Point2D<int> stopped_block, Point2D<int> stopped_chunk, Point2D<int> end_block, Point2D<int> end_chunk);
	static bool InitInterruptionHandler();
public:
	static exception_ptr interrupt;
	bool  IsOnSavedState();
	PathGenerator();
	PathGenerator(Location & location, Point2D<int> block, Point2D<int> chunk);
	~PathGenerator();
	void StartNew(Location & location, Point2D<int> start_block, Point2D<int> start_chunk, Point2D<int> end_block=Point2D<int>(63,63), Point2D<int> end_chunk= Point2D<int>(15, 15));
	void Continue();
	void Start(Location & location, Point2D<int> start_block, Point2D<int> start_chunk, Point2D<int> end_block, Point2D<int> end_chunk);
	static void LinkAllAreaChunks(NavArea & area);
	static void FindPathToChunk(Chunk & chunk);
	static void LinkTwoOrphanPoint(MapPoint & point1, MapPoint point2,vector<MapPoint> & points,  NavArea & area);
	static void FindNearestPoint(float * points, Point2D<float> & point);
	void LinkCentralChunkWithNeighbours();
	bool LinkTwoChunk(Chunk * chunk1,Chunk * chunk2, ChunksLink & link);
};

