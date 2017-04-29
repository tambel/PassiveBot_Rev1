#include "stdafx.h"
#include "Flags.h"


Flags::Flags()
{
}


Flags::~Flags()
{
}

Flag::Flag(const string & name, vector<shared_ptr<Region>> & regions)
{
	this->name = name;
	for (auto & region : regions)
	{
		
		if (region->GetType() == RegionType::FONT_STRING)
		{
			if (region->GetName() == this->name + "_flag_string")
			{
				this->region = region;
				break;
			}
		}
	}
	if (!this->region)
		throw(FlagError("Flag named: \"" + this->name + "\" cant find its fonstring"));
}

void Flag::Set()
{
	unsigned address = region->GetTextAddress();
	if (address)
		Process::Write<char>('1',address);
}

bool Flag::IsSet()
{
	unsigned address = region->GetTextAddress();
	bool result = false;
	if (address)
		try
	{
		result = Process::Read<char>(address) == '1' ? true : false;
	}
	catch (MemoryReadException & e)
	{
		result = false;
	}
	return result;
}

void Flag::SetAndWaitUnset()
{
	int attempts = 500;
	Set();
	while (IsSet() && attempts)
	{
		Sleep(10);
		attempts--;
	}
	if (!attempts)
	{
		throw(FlagError("Flag reached limit when waited unset"));
	}
	//flags_string.WriteByte('1', flags[name]);
}
