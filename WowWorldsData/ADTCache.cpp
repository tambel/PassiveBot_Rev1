#include "stdafx.h"
vector<ADT*> ADTCache::item_list=vector<ADT*>();
unsigned long ADTCache::list_size_limit=100;
ADT * ADTCache::Find(Location & location, Point2D<int> coordinates)
{

	for (auto adt:item_list)
	{
		if (adt->GetLocation()==location && adt->GetCoordinates()==coordinates)
		{
			return adt;
		}
	}
	return 0;
}
void ADTCache::Clear()
{
	//item_list.clear();

}
void ADTCache::Add(ADT * adt)
{
	Cache::Add(&item_list,adt,list_size_limit);
}