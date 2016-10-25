#pragma once
#include <vector>
#include "Adt.h"
#include "Cache.h"
#include "Utils.h"
using namespace std;
using namespace Game;
class ADT;
/*

class ADTCache
{
static vector<ADT*> adt_list;
static unsigned long size_limit;
public:
static void Add(ADT * adt);
static ADT * Find(string location, Point2D<int> coordinates);
static ADT * Find(ADT * adt) {return Find(adt->GetLocation(),adt->GetCoordinates());}

};
*/
class ADTCache
{
	static vector<ADT*> item_list;
	static unsigned long list_size_limit;
public:
	static void Add(ADT * adt);
	static ADT * Find(Location & location, Point2D<int> coordinates);
	//static ADT * Find(ADT * adt) {return Find(adt->GetLocation(),adt->GetCoordinates());}
	static void Clear();
};
