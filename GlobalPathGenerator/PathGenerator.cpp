#include "stdafx.h"
#include "PathGenerator.h"
#include "boost\filesystem.hpp"
#include <iostream>
#include <thread>
#include <limits>
using namespace std;
using namespace boost::filesystem;

exception_ptr PathGenerator::interrupt;
Point2D<int> PathGenerator::ConvertMapMerticsToGlobal(Point2D<int> block, Point2D<int> coords)
{
	int x =  16*block.X+coords.X;
	int y = 16 * block.Y + coords.Y;
	return Point2D<int>(x,y);
}
void PathGenerator::SaveState(Location & location, Point2D<int> stopped_block, Point2D<int> stopped_chunk, Point2D<int> end_block, Point2D<int> end_chunk)
{
	SavedState state;
	state.location_id = location.id;
	state.stopped_block = stopped_block;
	state.stopped_chunk = stopped_chunk;
	state.finish_block = end_block;
	state.finish_chunk = end_chunk;
	std::ofstream file;
	file.open("SavedState.txt", ios::binary);
	file.write(reinterpret_cast<char*>(&state), sizeof(SavedState));
}
#ifdef _WIN32
BOOL WINAPI consoleHandler(DWORD signal) {

	if (signal == CTRL_C_EVENT)
	{
		printf("Ctrl-C handled\n"); // do cleanup
		try
		{
			this_thread::sleep_for(chrono::seconds(1));
			throw(runtime_error("Ctrl+C pressed. Interrupting"));
		}
		catch(...)
		{
			PathGenerator::interrupt = current_exception();
		}
	}


	return TRUE;
}
#endif
bool PathGenerator::InitInterruptionHandler()
{
#ifdef _WIN32
	if (!SetConsoleCtrlHandler(consoleHandler, TRUE))
	{
		printf("\nERROR: Could not set control handler");
		return false;
	}
	return true;
#endif
}
bool PathGenerator::IsOnSavedState()
{

	return exists(path("SavedState.txt"));
	//return exists(path("generated_paths"));
}
PathGenerator::PathGenerator()
{
}
PathGenerator::PathGenerator(Location & location,Point2D<int> block,Point2D<int> coords)
{

	



}


PathGenerator::~PathGenerator()
{
}

void PathGenerator::StartNew(Location & location, Point2D<int> start_block, Point2D<int> start_chunk, Point2D<int> end_block, Point2D<int> end_chunk)
{

	Start(location, start_block, start_chunk, end_block, end_chunk);





	//std::ofstream file;
	//file.open("SavedState.txt");
	//SavedState s;
	//s.location_id=1234;
	//s.finish_block = Point2D<int>(5, 6);
	//s.finish_chunk = Point2D<int>(7, 8);
	//s.stopped_block = Point2D<int>(1, 2);
	//s.stopped_chunk = Point2D<int>(3, 4);
	//file.write(reinterpret_cast<char*>(&s), sizeof(s));
	//file.close();
}

void PathGenerator::Continue()
{
	std::ifstream file;
	//file.open("SavedState.txt");
	BinaryReader reader = BinaryReader("SavedState.txt");
	SavedState s= reader.Read<SavedState>();
	file.close();
	Start(Game::LocationBase::Get(s.location_id), s.stopped_block, s.stopped_chunk, s.finish_block, s.finish_chunk);
	
}

void PathGenerator::Start(Location & location, Point2D<int> start_block, Point2D<int> start_chunk, Point2D<int> end_block, Point2D<int> end_chunk)
{
	InitInterruptionHandler();
	Point2D<int>start = ConvertMapMerticsToGlobal(start_block, start_chunk);
	Point2D<int>end = ConvertMapMerticsToGlobal(end_block, end_chunk);
	Point2D<int> block;
	Point2D<int> chunk;
	unsigned s = 1024*start.X+start.Y;
	unsigned e= 1024 * end.X + end.Y;
	int x = 0;
	int y = 0;
	area = NavArea(1);
	try
	{
		for (unsigned i = s; i <= e; i++)
		{
			x = i / 1024;
			y = i - x * 1024;
			block.X = x / 16;
			chunk.X = x - block.X * 16;
			block.Y = y / 16;
			chunk.Y = y - block.Y * 16;
			try
			{
				area.UpdateArea(Game::LocationBase::Get("Kalimdor"), block, chunk);
				LinkCentralChunkWithNeighbours();
			}
			catch (EmptyAreaException & e)
			{
				cout << e.what() << endl;
			}
			if (interrupt)
			{
				rethrow_exception(interrupt);
			}
			cout << block.X << " " << block.Y << endl << chunk.X << " " << chunk.Y << endl << "==========" << endl;
		}
	}
	catch (...)
	{
		SaveState(location, block, chunk, end_block, end_chunk);

	}
}

void PathGenerator::LinkAllAreaChunks(NavArea & area)
{
	/*auto get_chunk_center=[](Chunk * chunk)
	{
		float * fv = &chunk->GetVertices()[216];
		return  Vector3(fv[0],fv[1],fv[2]);
	};*/
	auto get_chunk_center = [](Chunk * chunk)
	{
		return Vector3(chunk->GetGamePosition().x - Metrics::ChunkSize / 2, chunk->GetGamePosition().y - Metrics::ChunkSize / 2, chunk->GetGamePosition().z);
	};

	vector<MapPoint> points;
	for (auto &chunk : area.GetChunks())
	{
		//points.push_back(get_chunk_center(&*chunk));

		//Vector3 c = get_chunk_center2(&*chunk);
		//c= Vector3(Metrics::MapMidPoint - c.y, -(Metrics::MapMidPoint - c.x), c.z);

		MapPoint point;
		point.location_id = area.GetLocation().id;
		point.position = get_chunk_center(&*chunk);
		point.block = chunk->GetBlockCoordinates();
		point.chunk = chunk->GetCoordinates();
		points.push_back(point);
	}
	vector<int> v;
	for (auto &point : points)
	{
		for (auto &point2 : points)
		{
			if (&point != &point2)
			{
				area.FindPath(Vector3(Metrics::MapMidPoint - point.position.y, -(Metrics::MapMidPoint - point.position.x), point.position.z), Vector3(Metrics::MapMidPoint - point2.position.y, -(Metrics::MapMidPoint - point2.position.x), point2.position.z), 0);
				v.push_back(area.m_nsmoothPath);


			}
		}
	}
	std::ofstream file;
	file.open("points.csv",ios::binary);

	for (auto point : points)
	{
		file.write(reinterpret_cast<char*>(&point), sizeof(MapPoint));
	}

	/*for (auto point : points)
	{
		file << point.location_id << ", " << point.block.X << ", " << point.block.Y << ", " << point.chunk.X << ", " << point.chunk.Y;
		file << point.position.x << ", " << point.position.y << ", " << point.position.z << endl;
	}*/
	file.close();
}

void PathGenerator::FindPathToChunk(Chunk & chunk)
{
}

void PathGenerator::LinkTwoOrphanPoint(MapPoint & point1, MapPoint point2, vector<MapPoint> & points, NavArea & area)
{

}

void PathGenerator::FindNearestPoint(float * points, Point2D<float> & point)
{
	auto calc_distance = [](Point2D<float> & point1, Point2D<float> & point2)
	{
		return sqrtf(pow(point2.X - point1.X, 2)+ pow(point2.Y - point1.Y, 2));
	};
	/*auto calc_distance = [](Vector3 & point1, Vector3 & point2)
	{
	return sqrtf(pow(point2.x - point1.x, 2)+ pow(point2.y - point1.y, 2)+ pow(point2.z - point1.z, 2));
	};*/
	float min = 0.0f;
	min = (std::numeric_limits<float>::max)();
	Point2D<float> * min_point;
	for (int i = 0; i < 435; i += 3)
	{
		Point2D<float> * tmp_point = reinterpret_cast<Point2D<float>*>(&points[i]);
		float dist = calc_distance(*tmp_point, point);
		if (min > dist)
		{
			min_point = tmp_point;
			min = dist;
		}
	}

}

bool PathGenerator::LinkTwoChunk(Chunk * chunk1, Chunk * chunk2, ChunksLink & link)
{
	auto create_points_list = [](float * points)
	{
		vector<Vector3*> rpoints;
		for (int i = 0; i < 435; i += 3)
		{
			if (i == 216)
				continue;
			rpoints.push_back(reinterpret_cast<Vector3*>(&points[i]));
		}
		return rpoints;
	};
	auto points1 = move(create_points_list(chunk1->GetVertices()));
	auto points2 = move(create_points_list(chunk2->GetVertices()));
	Vector3 * most_central_point1 = reinterpret_cast<Vector3*>(&chunk1->GetVertices()[216]);
	Vector3 * most_central_point2 = reinterpret_cast<Vector3*>(&chunk2->GetVertices()[216]);


	auto find_nearest = [](vector<Vector3*> & points, Vector3 * point)
	{
		auto calc_distance = [](Vector3 * point1, Vector3 * point2)
		{
			return sqrtf(pow(point2->x - point1->x, 2) + pow(point2->y - point1->y, 2) + pow(point2->z - point1->z, 2));
		};
		float min_d = (numeric_limits<float>::max)();
		vector<Vector3*>::iterator min_it = points.begin();
		for (vector<Vector3*>::iterator it = points.begin(); it != points.end();)
		{
			float tmpd = calc_distance(point, *it);
			if (min_d > tmpd)
			{
				min_d = tmpd;
				min_it = it;
			}
			++it;
		}
		return min_it;

	};


	Vector3  p1 = *most_central_point1;
	Vector3 p2 = *most_central_point2;
	unsigned n_path = 0;
	float path[6144];
	bool first_turn = true;;
	vector<Vector3*>::iterator it = points1.end();
	auto current_points = &points1;
	bool first_time = true;
	area.m_nsmoothPath = 0;
	int c = 0;
	do
	{
		area.FindPath(p1, p2, 0);
		if (area.m_nsmoothPath)
		{
			if (c < 20)
			{
				
			}

			{
				//n_path = area.m_nsmoothPath;
				link.path.points_number = area.m_nsmoothPath;
				//link.path.points = new Vector3[link.path.points_number];
				memcpy(link.path.points, area.m_smoothPath, link.path.points_number * 12);
				MapPoint rp1, rp2;
				link.chunk1.position = p1;
				link.chunk1.location_id = area.GetLocation().id;
				link.chunk1.block = chunk1->GetBlockCoordinates();
				link.chunk1.chunk = chunk1->GetCoordinates();
				link.chunk2.position = p2;
				link.chunk2.location_id = area.GetLocation().id;
				link.chunk2.block = chunk2->GetBlockCoordinates();
				link.chunk2.chunk = chunk2->GetCoordinates();
				return true;
			}
		}
		if (first_time)
			first_time = false;
		else
			current_points->erase(it);
		if (first_turn)
		{
			it = find_nearest(points2, most_central_point2);
			p2 = **it;
			current_points = &points2;
			first_turn = false;
		}
		else
		{
			it = find_nearest(points1, most_central_point1);
			p1 = **it;
			current_points = &points1;
			first_turn = true;
		}
		c++;
	} while (points1.size() && points2.size());

	return false;
}

void PathGenerator::LinkCentralChunkWithNeighbours()
{
	Chunk * central_chunk = nullptr;
	for (auto &chunk : area.GetChunks())
	{
		if (chunk->GetBlockCoordinates() == area.GetBlockCoordinates() && chunk->GetCoordinates() == area.GetChunkCoordinates())
		{
			central_chunk = &*chunk;
			break;
		}
	}
	if (!central_chunk)
		return;//TODO return some eroor;
	vector<ChunksLink> links;
	ChunksLink link;
	for (auto &chunk : area.GetChunks())
	{
		if (&*chunk == central_chunk)
			continue;
		if (LinkTwoChunk(central_chunk, &*chunk, link))
		{
			links.push_back(link);
		}
	}
	std::ofstream file;
	file.open("links.bin",ios::binary);
	for (auto &link : links)
	{
		file.write(reinterpret_cast<char*>(&link), sizeof(ChunksLink));
	}
	//file.write(reinterpret_cast<char*>(&links[0]), sizeof(links.size()*sizeof(ChunksLink)));
	ChunksLink l[10];
	file.close();
	std::ifstream f;
	f.open("links.bin",ios::binary);
	f.read(reinterpret_cast<char*>(l), sizeof(l));
	f.close();
}
