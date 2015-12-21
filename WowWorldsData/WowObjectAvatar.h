#pragma once
#include "Doodad.h"
#include "WowInteraction\WowObject.h"
//using namespace Wow;
class WowObjectAvatar
{
public:
	Wow::WowObject * wow_object;
	Doodad * doodad;
	WowObjectAvatar();
	WowObjectAvatar(Wow::WowObject * wow_object);
	~WowObjectAvatar(void);
	Position   GetPosition();
};

