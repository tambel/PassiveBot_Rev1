// stdafx.h: включаемый файл дл€ стандартных системных включаемых файлов
// или включаемых файлов дл€ конкретного проекта, которые часто используютс€, но
// не часто измен€ютс€
//

#pragma once
#include <OgreSceneManager.h>
#include "Ogre.h"
#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreException.h>
#include <OgreEntity.h>
#include <OgreFrameListener.h>
#include <OgreWindowEventUtilities.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <SdkCameraMan.h>

#include <thread>
#include <mutex>

#include "WowWorldsData\NavArea.h"
#include "WowInteraction\WowObject.h"
#include "WowWorldsData\Model.h"
#include <WowInteraction\Utils.h>
#include <WowWorldsData\Utils.h>


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif