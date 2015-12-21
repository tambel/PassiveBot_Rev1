#pragma once
#include "WowObject.h"

namespace Wow
{
	class GameObject: public WowObject
	{

	public:
		GameObject(unsigned base);
		~GameObject(void);
		wchar_t *GetName(bool refresh=false);
		
	};
}

