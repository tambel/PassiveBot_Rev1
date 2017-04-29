#pragma once
#include "Tools\Tools.h"
#include "FrameManager.h"


class ComStringInitError : public runtime_error
{
public:
	ComStringInitError(const string & message) :runtime_error("ComStringInitError error: " + string(message)) {}
};

class OutComString
{
	unsigned address;
	unsigned size;
	bool unformatted;
public:
	OutComString();
	OutComString(Region * region, unsigned size,bool unformatted=false);
	OutComString(unsigned address, unsigned size);
	~OutComString();
	void Write(const string & str);
	void WriteByte(char value,unsigned index);
};

