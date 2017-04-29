#include "stdafx.h"
#include "OutComString.h"


OutComString::OutComString()
{
}

OutComString::OutComString(Region * region, unsigned size, bool unformatted)
{
	this->unformatted = unformatted;
	address = region->GetTextAddress();//Process::Read<unsigned>(region->GetBase() + WowOffsets2::FrameManager2::FontStringRegionText);
	this->size = size;
	if (address)
	{
		string str = move(Process::ReadASCII(address, 0));
		if (str.length() != this->size)
		{
			throw(ComStringInitError("Wrong fontstring size"));
		}
	}
	else
		throw(ComStringInitError("Cant read string from FontString region with name: " + region->GetName() + ". Bad Address"));
}

OutComString::OutComString(unsigned address, unsigned size)
{

}


OutComString::~OutComString()
{
}

void OutComString::Write(const string & str)
{
	if (unformatted)
	{
		if (str.length()<=size)
			Process::WriteRaw(const_cast<char*>(str.c_str()), str.length(), address);
	}
	else
	{
		auto pad_string = [](const string & str, unsigned length)
		{
			if (str.length() < length)
			{
				unsigned pad_size = length - str.length();
				char * pcstr = new char[pad_size];
				memset(pcstr, '&', pad_size);
				return str + string(pcstr, pad_size);

			}
		};
		unsigned str_length = str.length();
		if (str_length <= size)
		{

			string padded = pad_string(to_string(str_length) + ";" + str + "[!]", size);
			Process::WriteRaw(const_cast<char*>(padded.c_str()), padded.length(), address);
		}
	}
}

void OutComString::WriteByte(char value,unsigned index)
{
	if (index < size)
	{
		Process::Write<char>(value, address + index);
	}
}
