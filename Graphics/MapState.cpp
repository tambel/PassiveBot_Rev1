#include "stdafx.h"
#include "MapState.h"



MapState::MapState(const Ogre::String & helpDescription) :
	TutorialGameState(helpDescription)
{
}

MapState::~MapState()
{
}

void MapState::createScene01(void)
{
	TutorialGameState::createScene01();
}
