#include "stdafx.h"
vector<Chunk*> ChunkCache::item_list=vector<Chunk*>();
unsigned long ChunkCache::list_size_limit=2304;
Chunk * ChunkCache::Find(Location & location, Point2D<int> block_coordinates, Point2D<int>coordinates)
{
	for (auto chunk:item_list)
	{
		if (chunk->GetLocation()==location && chunk->GetBlockCoordinates()==block_coordinates && chunk->GetCoordinates()==coordinates)
		{
			return chunk;
		}
	}
	return 0;
}
void ChunkCache::Add(Chunk * chunk)
{
	Cache::Add(&item_list,chunk,list_size_limit);
}
