#pragma once
#include <exception>
#include <string>
using namespace  std;
class MemoryReadException : exception
{
	unsigned address;
public:
	MemoryReadException(unsigned address) :address(address) {}
	virtual const char * what()
	{
		return  string("Cant read memory at: " + to_string(address)).c_str();
	}
};
