#pragma once
#include "Cache.h"
#include "Chunk.h"
#include "Utils.h"
using namespace Utils;
class ChunkCache
{
	static vector<Chunk*> item_list;
	static unsigned long list_size_limit;
public:
	static Chunk * Find(Location *  location, Point2D<int> block_coordinates, Point2D<int>coordinates);
	static void Add(Chunk * chunk);

};

