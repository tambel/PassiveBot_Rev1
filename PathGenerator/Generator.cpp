#include "stdafx.h"
#include "Generator.h"

vector<int> Generator::chunk_vectors_order;
void Generator::init_order()
{
	for (int i = 1; i < 9; i++)
	{
		chunk_vectors_order.push_back(72);
		int start = 72 - 9 * i;
		for (int i2 = start; i2 <= start + i; i2++)
		{
			chunk_vectors_order.push_back(i2);
		}
		start = start + i;
		for (int i2 = start + 17; i2 <= start + 17 * i; i2 += 17)
		{
			chunk_vectors_order.push_back(i2);
		}
		start = start + 17 * i;
		for (int i2 = start - 1; i2 >= start - i; i2--)
		{
			chunk_vectors_order.push_back(i2);
		}
		start = start - i;
		for (int i2 = start - 17; i2 > start - 17 * i; i2 -= 17)
		{
			chunk_vectors_order.push_back(i2);
		}
	}
}

float Generator::CalcPathDistance(Vector3 * path, unsigned size)
{
	float result = 0.0;
	for (unsigned i = 0; i < size-1; i++)
	{
		result += CalcDistance(path[i], path[i+1]);
	}
	return result;
}

float Generator::CalcDistance(Vector3 & point1, Vector3 point2)
{
	float w= point1.x - point2.x;
	float h = point1.z - point2.z;
	return sqrtf(w*w + h*h);
}

bool Generator::IsPointsConnected(PathGeneration::Point * point1, PathGeneration::Point * point2)
{
	return point1->GetOppositePoint()->point == point2->point;
}

void Generator::ConnectPoints(PathGeneration::Point * point1, PathGeneration::Point * point2, Vector3 * path, unsigned path_size)
{
	links.push_back(unique_ptr<PathGeneration::Link>(new PathGeneration::Link()));
	PathGeneration::Link * link = links.back().get();
	if (path_size > 2)
	{
		for (int i = 1; i < path_size - 1; i++)
		{
			link->medium_points.push_back(path[i]);
		}
	}
	point1->link = link;
	point2->link = link;
	link->point1 = point1;
	link->point2 = point2;

}

Generator::Generator()
{
}


Generator::~Generator()
{
}

void Generator::Generate(Point2D<int> start_block, Point2D<int> rect)
{
	init_order();
	NavArea area;
	vector<Chunk*> neighbour_chunks=vector<Chunk*>();
	Chunk * center_chunk = nullptr;
	Vector3 c1_point, c2_point;
	bool path_found = false;
	for (int bx = start_block.X; bx < start_block.X + rect.X; bx++);
	{
		for (int by = start_block.Y; by < start_block.Y + rect.Y; by++);
		{
			for (int cx = 0; cx < 16; cx++);
			{
				for (int cy = 0; cy < 16; cy++);
				{
					area = NavArea(Game::LocationBase::Get("Kalimdor"), Point2D<int>(36, 32), Point2D<int>(7, 4), 1);
					for (auto & chunk : area.GetChunks())
					{
						if (chunk->GetCoordinates() != Point2D<int>(7, 4))
						{
							neighbour_chunks.push_back(chunk.get());
						}
						else
						{
							center_chunk = chunk.get();
						}
					}
					for (auto chunk : neighbour_chunks)
					{
						LinkToChunk(center_chunk, chunk, area);
					}
				}
			}
		}
	}
	int a;
	a = 10;
}

vector<unique_ptr<PathGeneration::Point>> * Generator::GetChunkPoints(Point2DI & block, Point2DI & coordinates)
{
	PathGeneration::Chunk * chunk = GetChunk(block, coordinates);
	if (chunk)
		return &chunk->points;
	return nullptr;
}

PathGeneration::Chunk * Generator::GetChunk(Point2DI & block, Point2DI & coordinates)
{
	for (auto & chunk : chunks)
	{
		if (chunk->block == block && chunk->coordinates == coordinates)
		{
			return chunk.get();
		}
	}
	chunks.push_back(unique_ptr<PathGeneration::Chunk>(new PathGeneration::Chunk()));
	chunks.back()->block = block;
	chunks.back()->coordinates = coordinates;
	chunks.back()->points = vector<unique_ptr<PathGeneration::Point>>();
	return chunks.back().get();
}

void Generator::LinkToChunk(Chunk * c1, Chunk * c2, NavArea & area)
{
	Vector3 * c1_point, *c2_point, *path;
	PathGeneration::Chunk * chunk1 = GetChunk(c1->GetBlockCoordinates(), c1->GetCoordinates());
	PathGeneration::Chunk * chunk2 = GetChunk(c2->GetBlockCoordinates(), c2->GetCoordinates());
	bool is_connected = false;
	bool stop_loop = false;
	if (chunk1 && chunk2)
	{
		for (auto & point : chunk1->points)
		{
			for (auto & point2 : chunk2->points)
			{
				if (IsPointsConnected(point.get(), point2.get()))
					continue;
				else
				{
					if (area.FindPath(point->point, point2->point, 0))
					{
						path = reinterpret_cast<Vector3*>(area.m_smoothPath);
						if (CalcPathDistance(path, area.m_nsmoothPath) > CalcDistance(*c1_point, *c2_point)*1.5)
							continue;
						ConnectPoints(point.get(), point2.get(), path, area.m_nsmoothPath);
						if (!is_connected)
							is_connected = true;

					}
				}
			}
		}
	}
	if (!is_connected)
	{
		for (auto & point : chunk2->points)
		{
			for (auto i : chunk_vectors_order)
			{
				c1_point = reinterpret_cast<Vector3*>(c1->GetVertices()) + i;
				c2_point = &point->point;
				if (area.FindPath(*c1_point, *c2_point, 0))
				{
					path = reinterpret_cast<Vector3*>(area.m_smoothPath);
					if (CalcPathDistance(path, area.m_nsmoothPath) > CalcDistance(*c1_point, *c2_point)*1.5)
						continue;
					chunk1->points.push_back(unique_ptr<PathGeneration::Point>(new PathGeneration::Point()));
					chunk1->points.back()->point = *c1_point;
					ConnectPoints(point.get(), chunk1->points.back().get(), path, area.m_nsmoothPath);
					if (!is_connected)
						is_connected = true;
					stop_loop = true;
					break;
				}
			}
			if (stop_loop)
				break;
		}
	}
	stop_loop = false;
	if (!is_connected)
	{
		for (auto i1 : chunk_vectors_order)
		{
			for (auto i2 : chunk_vectors_order)
			{
				c1_point = reinterpret_cast<Vector3*>(c1->GetVertices()) + i1;
				c2_point = reinterpret_cast<Vector3*>(c2->GetVertices()) + i2;
				if (area.FindPath(*c1_point, *c2_point, 0))
				{
					path = reinterpret_cast<Vector3*>(area.m_smoothPath);
					float ad = CalcPathDistance(path, area.m_nsmoothPath);
					float d = CalcDistance(*c1_point, *c2_point);
					if (CalcPathDistance(path, area.m_nsmoothPath) > CalcDistance(*c1_point, *c2_point)*1.5)
						continue;
					PathGeneration::Point * p1= new PathGeneration::Point();
					PathGeneration::Point * p2 = new PathGeneration::Point();
					p1->point = *c1_point;
					p2->point = *c2_point;

					chunk1->points.push_back(unique_ptr<PathGeneration::Point>(p1));
					chunk1->points.push_back(unique_ptr<PathGeneration::Point>(p2));
					ConnectPoints(p1, p2, path, area.m_nsmoothPath);

					/*chunk1->points.push_back(PathGeneration::Point());
					PathGeneration::Point * p1 = &chunk1->points.back();
					chunk1->points.push_back(PathGeneration::Point());
					PathGeneration::Point * p2 = &chunk1->points.back();
					p1->point = *c1_point;
					p2->point = *c2_point;*/
					
					if (!is_connected)
						is_connected = true;
					stop_loop = true;
					break;
				}
			}
			if (stop_loop)
				break;
		}
	}

}

