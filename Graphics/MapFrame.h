#include "GraphicsSystem.h"
#include "SdlInputHandler.h"

#include "OgreTimer.h"
#include "Threading/OgreThreads.h"
#include "OgreRenderWindow.h"

#include "OgreRoot.h"
#include "Compositor/OgreCompositorManager2.h"

//Declares WinMain / main
using namespace Demo;


class MapFrame : public GraphicsSystem
{
	virtual Ogre::CompositorWorkspace* setupCompositor();


public:
	MapFrame(GameState *gameState);
	int go();

};