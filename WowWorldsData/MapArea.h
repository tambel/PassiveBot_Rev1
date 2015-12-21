#pragma once
#include "Chunk.h"
#include <vector>
using namespace std;
class MapArea
{
protected:
	//Chunk ** chunks;
	Chunk *** chunks;
public:
	bool busy;
	MapArea(void);
	~MapArea(void);
	Chunk *** GetChunks();


};

