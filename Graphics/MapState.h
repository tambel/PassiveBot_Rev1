#pragma once
#include "OgrePrerequisites.h"
#include "TutorialGameState.h"

using namespace Demo;

class MapState : public TutorialGameState
{
public:
	MapState(const Ogre::String &helpDescription);
	~MapState();
	virtual void createScene01(void);
};


