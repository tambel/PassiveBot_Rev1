#pragma once
#include "Unit.h"
namespace Wow
{

	class Player:public Unit
	{
	public:
		Player(unsigned base);
		~Player(void);
		wstring & GetName(bool refresh=false);
	};
	
}

