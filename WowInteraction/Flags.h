#pragma once
#include "Tools\Tools.h"
#include "FrameManager.h"

class FlagError : public runtime_error
{
public:
	FlagError(const string & message) :runtime_error("ComStringInitError error: " + string(message)) {}
};

class Flag
{
	string name;
	shared_ptr<Region> region;
public:
	Flag(const string & name, vector<shared_ptr<Region>> & regions);
	~Flag() {};
	void Set();
	bool IsSet();
	void SetAndWaitUnset();


};


class Flags
{
public:
	Flags();
	~Flags();
};

