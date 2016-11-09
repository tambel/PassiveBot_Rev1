#pragma once
#include "WowWorldsData\WowWorldsData.h" 

namespace PathGeneration
{
	struct Point;
	struct Link
	{
		Point * point1;
		Point * point2;
		vector<Vector3> medium_points;
	};

	struct Point
	{
		
		Vector3 point;
		Link * link;
		Point * GetOppositePoint()
		{
			return link->point1->point == this->point ? link->point2 : link->point1;
		}
	};

	struct Chunk
	{
		Point2DI block;
		Point2DI coordinates;
		vector<unique_ptr<Point>> points;
	};
}
class Generator
{
	static vector<int> chunk_vectors_order;
	static void init_order();
	static float CalcPathDistance(Vector3 * path, unsigned size);
	static float CalcDistance(Vector3 & point1, Vector3 point2);
	static bool IsPointsConnected(PathGeneration::Point * point1, PathGeneration::Point * point2);
	void ConnectPoints(PathGeneration::Point * point1, PathGeneration::Point * point2, Vector3 * path, unsigned path_size);
	vector<unique_ptr<PathGeneration::Chunk>> chunks;
	vector<unique_ptr<PathGeneration::Link>> links;
public:
	Generator();
	~Generator();
	void Generate(Point2D<int> start_block, Point2D<int> end_block);
	vector<unique_ptr<PathGeneration::Point>> * GetChunkPoints(Point2DI & block,Point2DI & coordinates);
	PathGeneration::Chunk * GetChunk(Point2DI & block, Point2DI & coordinates);
	
	void LinkToChunk(Chunk * c1,Chunk * c2, NavArea & area);
};

