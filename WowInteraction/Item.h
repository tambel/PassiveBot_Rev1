#pragma once
#include "WowObject.h"

namespace Wow
{
	class Item: public WowObject
	{
	public:
		Item(unsigned base);
		~Item(void);
		wchar_t * GetName(bool refresh=false);
		
	};
}

