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

bool Generator::IsPointsConnected(Path::Point * point1, Path::Point * point2)
{
	return true;// point1->GetOppositePoint()->point == point2->point;
}

void Generator::ConnectPoints(Path::Point * point1, Path::Point * point2, Vector3 * path, unsigned path_size)
{
	links.push_back(unique_ptr<Path::Link>(new Path::Link()));
	Path::Link * link = links.back().get();
	if (path_size > 2)
	{
		for (int i = 1; i < path_size - 1; i++)
		{
			//link->points.push_back(path[i]);
		}
	}
	//point1->link = link;
	//point2->link = link;
	///link->point1 = point1;
	//link->point2 = point2;

}

Generator::Generator()
{
	area = nullptr;
	nm = GlobalNavMesh();
	nm.Start(0,0);
}


Generator::~Generator()
{
}

void Generator::LinkChunkWithNeighbours(Point2D<int> block, Point2D<int> coordinates)
{
	init_order();
	
	vector<Chunk*> neighbour_chunks = vector<Chunk*>();
	Chunk * center_chunk = nullptr;
	Vector3 c1_point, c2_point;
	bool path_found = false;
	if (area)
		area->UpdateArea(Game::LocationBase::Get("Kalimdor"), block, coordinates);
	else
		area =new  NavArea(Game::LocationBase::Get("Kalimdor"), block, coordinates, 1);
	//area.UpdateArea(Game::LocationBase::Get("Kalimdor"), block, coordinates);
	for (auto & chunk : area->GetChunks())
	{
		if (chunk->GetBlockCoordinates() == block && chunk->GetCoordinates() == coordinates)
		{
			center_chunk = chunk.get();
		}
		else
		{
			neighbour_chunks.push_back(chunk.get());
		}
	}
	for (auto chunk : neighbour_chunks)
	{
		LinkTwoChunk(center_chunk, chunk, *area);

	}
	int a;
	a = 10;
}

vector<Path::Point*> * Generator::GetChunkPoints(Point2DI & block, Point2DI & coordinates)
{
	Path::Chunk * chunk = GetChunk(block, coordinates);
	if (chunk)
		return &chunk->points;
	return nullptr;
}

Path::Chunk * Generator::GetChunk(Point2DI & block, Point2DI & coordinates)
{
	for (auto & chunk : chunks)
	{
		if (chunk->block == block && chunk->coordinates == coordinates)
		{
			return chunk.get();
		}
	}
	chunks.push_back(unique_ptr<Path::Chunk>(new Path::Chunk()));
	chunks.back()->block = block;
	chunks.back()->coordinates = coordinates;
	chunks.back()->points = vector<Path::Point*>();
	return chunks.back().get();
}

void Generator::LinkTwoChunk(Chunk * c1, Chunk * c2, NavArea & area)
{
	Vector3 * c1_point, *c2_point, * path;
	Path::Chunk * chunk1 = GetChunk(c1->GetBlockCoordinates(), c1->GetCoordinates());
	Path::Chunk * chunk2 = GetChunk(c2->GetBlockCoordinates(), c2->GetCoordinates());
	bool is_connected = false;
	bool stop_loop = false;
	/*
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
					chunk1->points.push_back(unique_ptr<Path::Point>(new Path::Point()));
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
	*/
	for (auto point : chunk1->points)
	{
		for (auto point2 : chunk2->points)
		{
			for (auto p2_link : point2->links)
			{
				if (p2_link->GetOppositePoint(point2) == point)
				{
					is_connected = true;
					break;
				}
			}
			if (is_connected)
				break;
		}
		if (is_connected)
			break;
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
				Vector3 coords = *c1_point;
				coords = Vector3(coords.x, coords.y, coords.z);
				Vector3 ucoords = *c2_point;
				ucoords = Vector3(ucoords.x, ucoords.y, ucoords.z);
				
				if (area.FindPath(coords, ucoords, 0)>=0)
				{
					path = reinterpret_cast<Vector3*>(area.m_smoothPath);
					float ad = CalcPathDistance(path, area.m_nsmoothPath);
					float d = CalcDistance(*c1_point, *c2_point);
					if (CalcPathDistance(path, area.m_nsmoothPath) > CalcDistance(*c1_point, *c2_point)*1.1);
						//continue;
					
					Path::Point * p1 = new Path::Point();
					Path::Point * p2 = new Path::Point();
					Path::Link * link = new Path::Link();
					link->point1 = p1;
					link->point2 = p2;
					p1->links.push_back(link);
					p2->links.push_back(link);
					
					link->size = area.m_nsmoothPath;
					p1->position = *path;
					p2->position = *(path + link->size - 1);
					if (link->size > 2)
					{
						link->points = unique_ptr<Vector3>(new Vector3[link->size]);
						memcpy(link->points.get(), path, (link->size) * 12);
					}
					else
						link->points = nullptr;
					links.push_back(unique_ptr<Path::Link>(link));
					points.push_back(unique_ptr<Path::Point>(p1));
					points.push_back(unique_ptr<Path::Point>(p1));
					chunk1->points.push_back(p1);
					chunk2->points.push_back(p2);

					is_connected = true;
					break;
				}
			}
			if (is_connected)
				break;
		}
	}

}

vector<unique_ptr<Path::Link>>& Generator::GetLinks()
{
	return links;
}

void Generator::AddBlockTiles()
{
}






