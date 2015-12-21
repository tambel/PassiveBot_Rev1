#pragma once
#include <fstream>
#include <string>
using namespace std;
class BinaryReader
{
	ifstream * stream;
public:
	BinaryReader(ifstream * stream);
	BinaryReader(string path);
	~BinaryReader(void);
	//Data reading
	unsigned int ReadUInt();
	unsigned int ReadUInt(unsigned long abs_position);
	void ReadBytes(char * buff,unsigned long abs_position,unsigned long length);
	template <class T>
	void ReadArray(void * buff, unsigned long count=1)
	{
		stream->read((char*)buff,count*sizeof(T));
	}
	template <class T>
	void ReadArrayAbs(void * buff, unsigned long offset, unsigned long count=1)
	{

		stream->seekg(offset,stream->beg);
		stream->read((char*)buff,count*sizeof(T));
	}
	template <class T>
	T Read()
	{
		T result;
		stream->read((char*)&result,sizeof(T));
		return result;
	}
	//
	void MoveToBegining();
	void MoveForward(unsigned long offset);
	void MoveBackward(unsigned long offset);
	void SetPosition(unsigned long position);
	unsigned long GetPosition();
	ifstream * GetStream();
	bool IsFileExist();
};

