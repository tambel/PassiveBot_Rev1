#pragma once
#include <fstream>
#include <string>
using namespace std;

class BinaryReaderError :public runtime_error
{
public:
	using runtime_error::runtime_error;
};
class BinaryReaderFileNotExist :public BinaryReaderError
{
	using BinaryReaderError::BinaryReaderError;
};



class BinaryReader
{
	int i;
	ifstream stream;
	void _move(BinaryReader & other);
public:
	BinaryReader() {}
	BinaryReader(ifstream * stream);
	BinaryReader(string path);
	BinaryReader(BinaryReader && other);
	~BinaryReader(void);
	BinaryReader & operator=(BinaryReader && other);
	//Data reading
	unsigned int ReadUInt();
	unsigned int ReadUInt(unsigned long abs_position);
	void ReadBytes(char * buff,unsigned long abs_position,unsigned long length);
	template <class T>
	void ReadArray(void * buff, unsigned long count=1)
	{
		stream.read((char*)buff,count*sizeof(T));
	}
	template <class T>
	void ReadArrayAbs(void * buff, unsigned long offset, unsigned long count=1)
	{

		stream.seekg(offset,stream.beg);
		stream.read((char*)buff,count*sizeof(T));
	}
	template <class T>
	T Read()
	{
		T result;
		stream.read((char*)&result,sizeof(T));
		return result;
	}
	//
	void MoveToBegining();
	void MoveForward(unsigned long offset);
	//void MoveBackward(unsigned long offset);
	void SetPosition(unsigned long long position);
	unsigned long long GetPosition();
	ifstream * GetStream();
	bool IsFileExist();
};

