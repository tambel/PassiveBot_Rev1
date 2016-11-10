#pragma once
#include <fstream>
using namespace std;

class BinaryReaderError :public runtime_error
{
public :
	BinaryReaderError(string message) : runtime_error(message) {}
};

class BinaryReader
{
	unique_ptr<ifstream> stream;
public:
	BinaryReader() {}
	BinaryReader(BinaryReader && reader);
	BinaryReader(ifstream * stream);
	BinaryReader(string path);
	~BinaryReader(void);
	//Data reading
	//unsigned int ReadUInt();
	//unsigned int ReadUInt(unsigned long abs_position);
	//void ReadBytes(char * buff,unsigned long abs_position,unsigned long length);
	template <class T>
	void ReadArray(void * buff, unsigned long count=1)
	{
		try
		{
			stream->read((char*)buff, count*sizeof(T));
		}
		catch (fstream::failure & e)
		{
			throw_with_nested(BinaryReaderError("BinaryReader::ReadArray failed. Stream read failure"));

		}
		
	}
	template <class T>
	void ReadArrayAbs(void * buff, unsigned long offset, unsigned long count=1)
	{
		try
		{
			stream->seekg(offset, stream->beg);
			stream->read((char*)buff, count*sizeof(T));
		}
		catch (fstream::failure & e)
		{
			throw_with_nested(BinaryReaderError("BinaryReader::ReadArrayAbs failed. Stream read failure"));

		}
	}
	template <class T>
	T Read()
	{
		
		try
		{
			T result;
			stream->read((char*)&result, sizeof(T));
			return result;
		}
		catch (fstream::failure & e)
		{
			throw_with_nested(BinaryReaderError("BinaryReader::Read failed. Stream read failure"));

		}
	}
	//
	void MoveToBegining();
	void MoveForward(unsigned long offset);
	//void MoveBackward(unsigned long offset);
	void SetPosition(unsigned long long position);
	string ReadString(unsigned count=0);
	unsigned long long GetPosition();
	ifstream * GetStream();
	bool IsFileExist();
};

