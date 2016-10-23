#include "stdafx.h"
BinaryReader::BinaryReader(ifstream * stream)
{
	this->stream=stream;
}
BinaryReader::BinaryReader(string path)
{
	this->stream=new ifstream(path, ios::binary);

}

BinaryReader::~BinaryReader(void)
{
	this->stream->close();
	delete this->stream;
}
unsigned int BinaryReader::ReadUInt()
{
	unsigned int result;
	stream->read((char*)&result,4);
	return result;
}
unsigned int BinaryReader::ReadUInt(unsigned long abs_position)
{
	unsigned long long  old_position=stream->tellg();
	unsigned int result;
	stream->seekg (abs_position, stream->beg);
	stream->read((char*)&result,4);
	stream->seekg(old_position,ios::beg);
	return result;
}
void BinaryReader::ReadBytes(char * buff,unsigned long abs_position,unsigned long length)
{
	unsigned long long  old_position=stream->tellg();
	stream->seekg (abs_position, stream->beg);
	stream->read(buff,length);
	stream->seekg(old_position,ios::beg);
}


void BinaryReader::MoveToBegining()
{
	stream->seekg(0,stream->beg);
}
void BinaryReader::MoveForward(unsigned long offset)
{
	stream->seekg(offset,ios::cur);
}
ifstream * BinaryReader::GetStream()
{
	return stream;
}
void BinaryReader::SetPosition(unsigned long long position)
{
	stream->seekg(position,ios::beg);
}
string BinaryReader::ReadString(unsigned count)
{
	char * buff = new char[count+1];
	ReadArray<char>(buff,count);
	if (buff[count] != 0)
	{
		buff[count] = 0;
	}
	return string(buff);
}
unsigned long long BinaryReader::GetPosition()
{ 

	return stream->tellg();
}
bool BinaryReader::IsFileExist()
{
	if (stream)
	{
		if (stream->good())
		{
			return true;
		}
	}
	return false;
}