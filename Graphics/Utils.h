#pragma once
#include "Ogre.h"
#include <WowLib\Utils.h>
#include <WowDataLib\Utils.h>
using namespace Utils;
namespace Utils
{
	namespace OgreGraphics
	{

	}
}
Ogre::Vector3 Vector3ToOgreVector(WowTypes::Vector3 vector);
Ogre::ColourValue ColorToOgreColor(Graphics::Color color);

