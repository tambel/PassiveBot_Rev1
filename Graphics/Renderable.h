#pragma once
#include "OgreSceneManager.h"
#include "Utils.h"
#include "WowWorldsData\Model.h"
class Renderable
{

	Model * model;
	static unsigned long counter;
	unsigned long id;
	Ogre::SceneNode * scene;
public:
	bool to_kill;
	Renderable(Model * model);
	~Renderable(void);
	bool CreateScene(Ogre::SceneNode * parent);
	unsigned long GetID() {return id;}
	Ogre::SceneNode * GetScene() {return scene;}
	Model * GetModel() {return model;}

};


