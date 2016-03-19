#include "stdafx.h"
void BinaryReader::_move(BinaryReader & other)
{
	i = other.i;
	other.i = 0;
	this->stream = move(other.stream);
}
BinaryReader::BinaryReader(ifstream * stream)
{
	//this->stream=stream;
}
BinaryReader::BinaryReader(string path)
{
	i = 15;
	stream.exceptions(ifstream::failbit | ifstream::badbit | ifstream::eofbit);
	try
	{
		//this->stream = ifstream(path, ios::binary);
		stream.open(path, ios::binary);
	}
	catch (...)
	{
		throw_with_nested(BinaryReaderError("BinaryReader initialization failure. File:" + path));
	}
		
}
BinaryReader::BinaryReader(BinaryReader && other)
{
	_move(other);
}

BinaryReader::~BinaryReader(void)
{
	this->stream.close();
}
BinaryReader & BinaryReader::operator=(BinaryReader && other)
{
	_move(other);
	return *this;
}
unsigned int BinaryReader::ReadUInt()
{
	unsigned int result;
	stream.read((char*)&result,4);
	return result;
}
unsigned int BinaryReader::ReadUInt(unsigned long abs_position)
{
	unsigned long long  old_position=stream.tellg();
	unsigned int result;
	stream.seekg (abs_position, stream.beg);
	stream.read((char*)&result,4);
	stream.seekg(old_position,ios::beg);
	return result;
}
void BinaryReader::ReadBytes(char * buff,unsigned long abs_position,unsigned long length)
{
	unsigned long long  old_position=stream.tellg();
	stream.seekg (abs_position, stream.beg);
	stream.read(buff,length);
	stream.seekg(old_position,ios::beg);
}


void BinaryReader::MoveToBegining()
{
	stream.seekg(0,stream.beg);
}
void BinaryReader::MoveForward(unsigned long offset)
{
	stream.seekg(offset,ios::cur);
}
ifstream * BinaryReader::GetStream()
{
	return &stream;
}
void BinaryReader::SetPosition(unsigned long long position)
{
	stream.seekg(position,ios::beg);
	
}
unsigned long long BinaryReader::GetPosition()
{ 

	return stream.tellg();
}
bool BinaryReader::IsFileExist()
{
	if (stream)
	{
		if (stream.good())
		{
			return true;
		}
	}
	return false;
}