#include "stdafx.h" 
#include "MapFrame.h"


#include "OgreConfigFile.h"
Ogre::CompositorWorkspace * MapFrame::setupCompositor()
{
	
	Ogre::CompositorManager2 *compositorManager = mRoot->getCompositorManager2();
	return compositorManager->addWorkspace(mSceneManager, mRenderWindow, mCamera,
		"ShadowMapDebuggingWorkspace", true);
}

MapFrame::MapFrame(GameState * gameState):
	GraphicsSystem(gameState)
{
	game_state = gameState;
}


void MapFrame::setupResources(void)
{
	GraphicsSystem::setupResources();

	Ogre::ConfigFile cf;

	cf.load(mResourcePath + "resources2.cfg");

	Ogre::String dataFolder = cf.getSetting("DoNotUseAsResource", "Hlms", "");

	if (dataFolder.empty())
		dataFolder = "./";
	else if (*(dataFolder.end() - 1) != '/')
		dataFolder += "/";

	dataFolder += "2.0/scripts/materials/simple";

	addResourceLocation(dataFolder, "FileSystem", "General");

}

int MapFrame::go()
{
	initialize("Using V1 interfaces directly");

	if (getQuit())
	{
		deinitialize();
		return 0; //User cancelled config
	}

	Ogre::RenderWindow *renderWindow = getRenderWindow();

	createScene01();
	createScene02();

	//Do this after creating the scene for easier the debugging (the mouse doesn't hide itself)
	SdlInputHandler *inputHandler = getInputHandler();
	inputHandler->setGrabMousePointer(true);
	inputHandler->setMouseVisible(false);

	Ogre::Timer timer;
	unsigned long startTime = timer.getMicroseconds();

	double timeSinceLast = 1.0 / 60.0;

	while (!getQuit())
	{
		beginFrameParallel();
		update(static_cast<float>(timeSinceLast));
		finishFrameParallel();
		finishFrame();

		if (!renderWindow->isVisible())
		{
			//Don't burn CPU cycles unnecessary when we're minimized.
			
			Ogre::Threads::Sleep(500);
		}

		unsigned long endTime = timer.getMicroseconds();
		timeSinceLast = (endTime - startTime) / 1000000.0;
		timeSinceLast = std::min(1.0, timeSinceLast); //Prevent from going haywire.
		startTime = endTime;
	}

	destroyScene();
	deinitialize();

	return 0;
}
