#include "stdafx.h"
#include "PathGenerator.h"
#include "boost\filesystem.hpp"
#include <iostream>
#include <thread>
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
		//throw(current_exception());

	}
}
