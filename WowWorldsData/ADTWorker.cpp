#include "stdafx.h"
#include "ADTWorker.h"
vector<shared_ptr<ADT>> ADTWorker::adts;

Chunk * ADTWorker::GetChunk(Area * area,Location & location, Point2D<int> block_coordinates, Point2D<int>coordinates,bool save)
{
	Chunk * chunk=ChunkCache::Find(location,block_coordinates,coordinates);
	if (chunk)
		return chunk;
	//shared_ptr<ADT>  adt=0;
	shared_ptr<ADT>  adt=GetADT(location, block_coordinates);
	if (!adt)
		return nullptr;
	
	chunk= adt->GetChunk(area,coordinates);
	//chunk->SetADTptr(adt);
	if (chunk)
	{
		//if (save)
			//ChunkCache::Add(chunk);
		return chunk;
	}
	return nullptr;
}
shared_ptr<ADT> ADTWorker::GetADT(Location & location, Point2D<int> coordinates)
{
	shared_ptr<ADT> result=nullptr;
	for (auto &adt_ptr : adts)
	{
		if (adt_ptr->GetLocation() == location && adt_ptr->GetCoordinates() == coordinates)
		{
			return adt_ptr;
		}
	}
	try
	{
		//ADT * adt = new ADT(location, coordinates);
		adts.push_back(make_shared<ADT>(location, coordinates));
		return adts.back();
	}
	catch (...)
	{
		return nullptr;
	}
	for (vector<shared_ptr<ADT>>::iterator it = adts.begin(); it != adts.end();)
	{
		if (it->use_count() < 2)
		{
			it = adts.erase(it);
		}
		else
		{
			++it;
		}
	}
	/*for (vector<shared_ptr<ADT>>::iterator it = adts.begin(); it != adts.end();)
	{
		if ((*it)->GetLocation() == location && (*it)->GetCoordinates() == coordinates)
		{
			
		}
	}*/
}

void ADTWorker::Clear()
{
	for (auto &adt : adts)
	{
		adt.reset();
	}
	adts.clear();
	adts.shrink_to_fit();
}
