// stdafx.h: включаемый файл дл€ стандартных системных включаемых файлов
// или включаемых файлов дл€ конкретного проекта, которые часто используютс€, но
// не часто измен€ютс€
//

#pragma once
#include "WowWorldsData\stdafx.h"
#include "MapFrame.h"
#include "Renderable.h"
#include "Utils.h"
#include <OgreSceneManager.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif