#include "stdafx.h"
#include "BinaryReader.h"
BinaryReader::BinaryReader(ifstream * stream)
{
	this->stream=unique_ptr<ifstream>(stream);
}
BinaryReader::BinaryReader(string path)
{
	try
	{
		this->stream = unique_ptr<ifstream>(new ifstream(path, ios::binary));
		if (stream)
		{
			if (!stream->good())
			{
				throw(ifstream::failure("\"good\" function failed"));
			}
		}
	}
	catch (fstream::failure & e)
	{
		throw_with_nested(BinaryReaderError("BinaryReader initialization failed. Stream failure"));
	}
}

BinaryReader::BinaryReader(BinaryReader && reader)
{
	this->stream =move(reader.stream);
}

BinaryReader::~BinaryReader(void)
{
	try
	{
		this->stream->close();
	}
	catch (fstream::failure & e)
	{
		throw_with_nested(BinaryReaderError("BinaryReader destructor failed. Stream failure"));

	}
	
}
//unsigned int BinaryReader::ReadUInt()
//{
//	unsigned int result;
//	stream->read((char*)&result,4);
//	return result;
//}
//unsigned int BinaryReader::ReadUInt(unsigned long abs_position)
//{
//	unsigned long long  old_position=stream->tellg();
//	unsigned int result;
//	stream->seekg (abs_position, stream->beg);
//	stream->read((char*)&result,4);
//	stream->seekg(old_position,ios::beg);
//	return result;
//}
//void BinaryReader::ReadBytes(char * buff,unsigned long abs_position,unsigned long length)
//{
//	unsigned long long  old_position=stream->tellg();
//	stream->seekg (abs_position, stream->beg);
//	stream->read(buff,length);
//	stream->seekg(old_position,ios::beg);
//}


void BinaryReader::MoveToBegining()
{
	try
	{
		stream->seekg(0,stream->beg);
	}
	catch (fstream::failure & e)
	{
		throw_with_nested(BinaryReaderError("BinaryReader::MoveToBegining failed. Stream failure"));

	}
	
}
void BinaryReader::MoveForward(unsigned long offset)
{
	
	try
	{
		stream->seekg(offset, ios::cur);
	}
	catch (fstream::failure & e)
	{
		throw_with_nested(BinaryReaderError("BinaryReader::MoveForward failed. Stream failure"));

	}

}
ifstream * BinaryReader::GetStream()
{
	return &*stream;
}
void BinaryReader::SetPosition(unsigned long long position)
{
	
	try
	{
		stream->seekg(position, ios::beg);
	}
	catch (fstream::failure & e)
	{
		throw_with_nested(BinaryReaderError("BinaryReader::SetPosition failed. Stream failure"));

	}
}
string BinaryReader::ReadString(unsigned count)
{
	try
	{
		if (count == 0)
		{
			string result;;
			char c = 0;
			stream->read(&c, 1);
			while (c)
			{
				result += c;
				stream->read(&c, 1);
				
			}
			return result;
		}
		unique_ptr<char>  buff = unique_ptr<char>(new char[count + 1]);
		ReadArray<char>(buff.get(), count);
		if (buff.get()[count] != 0)
		{
			buff.get()[count] = 0;
		}
		return string(buff.get());
	}
	catch (fstream::failure & e)
	{
		throw_with_nested(BinaryReaderError("BinaryReader::ReadString failed. Stream failure"));
	}
	catch (BinaryReaderError & e)
	{
		throw(BinaryReaderError("BinaryReader::ReadString failed. " + string(e.what())));
	}
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