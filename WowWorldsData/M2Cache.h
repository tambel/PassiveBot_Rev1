#pragma once
#include "Cache.h"
#include <vector>
#include "M2.h"
using namespace std;
class M2Cache
{
	vector<M2*> item_list;
	unsigned long size_limit;
public:
	M2Cache(void);
	~M2Cache(void);
};

