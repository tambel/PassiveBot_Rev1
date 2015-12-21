#pragma once
#include "WowObject.h"
#include "Utils.h"
using namespace Utils::WowTypes; 
namespace Wow
{
	class Unit: public WowObject
	{
	public:
		Unit(unsigned base);
		~Unit(void);
		wchar_t * GetName(bool refresh=false);
		Position GetPosition();
		void DumpPosition();
	};
}

