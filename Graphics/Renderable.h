#pragma once
#include "OgreSceneManager.h"
#include "Utils.h"
#include "WowWorldsData\Model.h"
class Renderable
{

	Model<unsigned short> * model;
	static unsigned long counter;
	unsigned long id;
	Ogre::SceneNode * scene;
public:
	bool to_kill;
	Renderable(Model<unsigned short> * model);
	~Renderable(void);
	bool CreateScene(Ogre::SceneNode * parent);
	unsigned long GetID() {return id;}
	Ogre::SceneNode * GetScene() {return scene;}
	Model<unsigned short> * GetModel() {return model;}

};


