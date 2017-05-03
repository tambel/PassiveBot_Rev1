#include "stdafx.h"
#include "DB2Reader.h"


DB2Reader::DB2Reader(string & filename ,bool cache_strings):cache_strings(cache_strings), filename(filename)
{
}


DB2Reader::~DB2Reader()
{
	delete[] strings;
	strings = nullptr;
}

string DB2Reader::ReadString(unsigned ofs)
{
	return string();
}
