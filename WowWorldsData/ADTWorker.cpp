#include "stdafx.h"

Chunk * ADTWorker::GetChunk(Location * location, Point2D<int> block_coordinates, Point2D<int>coordinates,bool save)
{
	Chunk * chunk=ChunkCache::Find(location,block_coordinates,coordinates);
	if (chunk)
		return chunk;
	ADT * adt=0;
	adt=GetADT(location, block_coordinates);
	if (!adt->IsExist())
		return 0;
	chunk= adt->GetChunk(coordinates);
	if (chunk)
	{
		if (save)
			ChunkCache::Add(chunk);
		return chunk;
	}
}
ADT * ADTWorker::GetADT(Location * location, Point2D<int> coordinates)
{

	ADT * adt=0;
	adt=ADTCache::Find(location,coordinates);
	if (!adt)
	{
		adt=new ADT(location,coordinates);
		ADTCache::Add(adt);
	}
	if (!adt->IsExist())
	{
		delete adt;
		return 0;
	}
	return adt;
}
