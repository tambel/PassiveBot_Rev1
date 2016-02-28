#pragma once
#include "WowObject.h"
#include "Utils.h"
#include <string>
using namespace std;
using namespace Utils;

class Unit : public WowObject
{
public:
	Unit(unsigned base);
	~Unit(void);
	wstring & GetName(bool refresh = false);
	Position GetPosition();
	void DumpPosition();
};


