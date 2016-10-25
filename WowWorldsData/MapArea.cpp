#include "stdafx.h"
#include "MapArea.h"

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
