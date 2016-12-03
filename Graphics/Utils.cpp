#include "stdafx.h" 
#include "Utils.h"

Ogre::Vector3 Vector3ToOgreVector(Vector3 vector)
{
	return Ogre::Vector3(vector.x, vector.y, vector.z);
}
Ogre::ColourValue ColorToOgreColor(Graphics::Color color)
{
	return Ogre::ColourValue(color.r, color.g, color.b, color.a);
}

