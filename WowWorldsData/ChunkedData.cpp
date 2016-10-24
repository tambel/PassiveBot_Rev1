#include "stdafx.h"

ChunkedData::ChunkedData(void)
{
}


ChunkedData::~ChunkedData(void)
{
}

bool ChunkedData::SeekChunk(BinaryReader * reader,unsigned int signature,bool begin)
{

	if (begin)
	{
		reader->MoveToBegining();
	}
	unsigned int sig;
	sig=reader->Read<unsigned>();
	
	while (!reader->GetStream()->eof() && sig!=signature)
	{
		unsigned long long pos = reader->GetPosition();
		unsigned int size=reader->Read<unsigned>();
		reader->MoveForward(size);
		//reader->GetStream()->seekg(size,ios::cur);
		sig=reader->Read<unsigned>();
	}
	return 0;
	
}

bool ChunkedData::SeekChunk(BinaryReader & reader, unsigned int signature, bool begin)
{
	if (begin)
	{
		reader.MoveToBegining();
	}
	unsigned int sig;
	sig = reader.ReadUInt();

	while (!reader.GetStream()->eof() && sig != signature)
	{
		unsigned int size = reader.ReadUInt();
		reader.MoveForward(size);
		//reader->GetStream()->seekg(size,ios::cur);
		sig = reader.ReadUInt();
	}
	if (!reader.GetStream()->eof())
		return true;
	else
		return false;
}
