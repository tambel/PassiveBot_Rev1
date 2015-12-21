#include "stdafx.h"

MapArea::MapArea(void)
{
	busy=false;
}


MapArea::~MapArea(void)
{
}
Chunk *** MapArea::GetChunks()
{
	return chunks;
}
