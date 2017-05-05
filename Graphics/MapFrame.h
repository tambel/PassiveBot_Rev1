#pragma once
#include "GraphicsSystem.h"
#include "SdlInputHandler.h"

#include "OgreTimer.h"
#include "Threading/OgreThreads.h"
#include "OgreRenderWindow.h"

#include "OgreRoot.h"
#include "Compositor/OgreCompositorManager2.h"

#include "MapState.h"


//Declares WinMain / main
using namespace Demo;


class MapFrame : public GraphicsSystem
{
	MapState * game_state;
	virtual Ogre::CompositorWorkspace* setupCompositor();
	virtual void setupResources(void);

public:
	MapFrame(MapState *gameState);
	int go();

};