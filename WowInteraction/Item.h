#pragma once
#include "WowObject.h"
using namespace std;
namespace Wow
{
	class Item: public WowObject
	{
	public:
		Item(unsigned base);
		~Item(void);
		wstring & GetName(bool refresh=false);
		
	};
}

