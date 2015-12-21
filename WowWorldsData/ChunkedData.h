#pragma once
#include <Tools\BinaryReader.h>
using namespace std;
struct ChunkStreamInfo
{
	BinaryReader * reader;
	unsigned long start;
	unsigned long size;
	ChunkStreamInfo():reader(0),start(0),size(0){}
	ChunkStreamInfo(unsigned long position,unsigned long size,BinaryReader * reader) 
	{
		start=position;
		this->size=size;
		this->reader=reader;
	}
};
class ChunkedData
{
public:
	ChunkedData(void);
	~ChunkedData(void);
	static bool SeekChunk(BinaryReader * reader, unsigned int signature, bool begin=false);
};

