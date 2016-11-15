#pragma once
#include "Utils.h"

class Renderable
{
protected:
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
	virtual void CreateScene(Ogre::SceneNode * parent, Ogre::ColourValue & color = Ogre::ColourValue(1, 1, 0, 1));

	unsigned long GetID() {return id;}
	Ogre::SceneNode * GetScene() {return scene;}
	Model * GetModel() {return model;}
	
};

class LineStripRenderable : public Renderable
{
	Vector3 * points = nullptr;
	unsigned size = 0;
public :
	LineStripRenderable(LineStripRenderable && other);
	LineStripRenderable(Vector3 * points, unsigned size);
	virtual void CreateScene(Ogre::SceneNode * parent);
};


class NavMeshRenderable : public Renderable
{
	vector<rcPolyMesh*> * meshes;
public:
	NavMeshRenderable(NavMeshRenderable && other);
	NavMeshRenderable(vector<rcPolyMesh*> & meshes);
	void CreateScene(Ogre::SceneNode * parent, Ogre::ColourValue & color = Ogre::ColourValue(1, 1, 0, 1));
};



