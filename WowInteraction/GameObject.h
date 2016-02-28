#pragma once
#include "WowObject.h"


class GameObject : public WowObject
{

public:
	GameObject(unsigned base);
	~GameObject(void);
	wstring & GetName(bool refresh = false);

};


