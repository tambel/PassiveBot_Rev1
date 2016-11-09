#pragma once
#include "Utils.h"

class Renderable
{

	Model * model=nullptr;
	static unsigned long counter;
	unsigned long id;
	Ogre::SceneNode * scene=nullptr;
	void _move(Renderable & other);
	Ogre::ManualObject * manual_object = nullptr;
public:
	bool to_kill;
	Renderable();
	Renderable(Model * model);

	
	
	Renderable(const Renderable &)=default  ;
	Renderable(Renderable &&);
	Renderable & operator=(const Renderable &)=default;
	Renderable & operator=(Renderable &&);
	
	~Renderable();
	void CreateScene(Ogre::SceneNode * parent);
	unsigned long GetID() {return id;}
	Ogre::SceneNode * GetScene() {return scene;}
	Model * GetModel() {return model;}
	
};


