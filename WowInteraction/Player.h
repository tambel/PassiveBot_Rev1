#pragma once
#include "Unit.h"

class Player :public Unit
{
public:
	Player(unsigned base);
	~Player(void);
	virtual const wstring & GetName(bool refresh = false);
};



