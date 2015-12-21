/*
#pragma once
#include "OgreSceneManager.h"
#include "Utils.h"
#include "WowDataLib\Model.h"
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
	void CreateScene(Ogre::SceneNode * parent);
	unsigned long GetID() {return id;}
	Ogre::SceneNode * GetScene() {return scene;}
	Model * GetModel() {return model;}

};
*/
