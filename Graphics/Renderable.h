#pragma once
#include "Utils.h"

enum DrawNavMeshFlags
{
	DU_DRAWNAVMESH_OFFMESHCONS = 0x01,
	DU_DRAWNAVMESH_CLOSEDLIST = 0x02,
	DU_DRAWNAVMESH_COLOR_TILES = 0x04,
};

class Renderable
{
protected:
	
	static unsigned long counter;
	unsigned long id;
	Ogre::SceneNode * scene = nullptr;
	void _move(Renderable & other);
	Ogre::ManualObject * manual_object = nullptr;
	Ogre::SceneNode * parent;
	string material;
	Ogre::ColourValue color;
	
public:
	bool to_kill;
	Renderable();
	Renderable(string & material);
	Renderable(const Renderable &) = default;
	Renderable(Renderable &&);
	Renderable & operator=(const Renderable &) = default;
	Renderable & operator=(Renderable &&);

	~Renderable();
	virtual void NewCreateScene(Ogre::SceneNode * parent) {};

	unsigned long GetID() { return id; }
	Ogre::SceneNode * GetScene() { return scene; }
	
};

class WowModelRenderable :public Renderable
{
	Model * model = nullptr;
public:
	//WowModelRenderable();
	WowModelRenderable(Model * model, string & material);
	WowModelRenderable(const WowModelRenderable &) = default;
	WowModelRenderable(WowModelRenderable && other);
	~WowModelRenderable() {};
	Model * GetModel() { return model; }
	void NewCreateScene(Ogre::SceneNode * parent);
};

class LineStripRenderable : public Renderable
{
	Vector3 * points = nullptr;
	vector<Vector3> vpoints;
	unsigned size = 0;
public:
	LineStripRenderable(LineStripRenderable && other);
	LineStripRenderable(Vector3 * points, unsigned size);
	LineStripRenderable(vector<Vector3> & points);
	~LineStripRenderable() {};
	void NewCreateScene(Ogre::SceneNode * parent);
};


class NavMeshRenderable : public Renderable
{
	vector<rcPolyMesh*> * meshes;
	dtNavMesh * mesh;
	Vector3 offset;
public:
	NavMeshRenderable(NavMeshRenderable && other);
	NavMeshRenderable(vector<rcPolyMesh*> & meshes);
	NavMeshRenderable(dtNavMesh * mesh, Vector3 offset = Vector3(0, 0, 0));
	void DrawNavMesh(const dtNavMesh& mesh, const dtNavMeshQuery* query, unsigned char flags, string & material);
	void drawMeshTile(const dtNavMesh& mesh, const dtNavMeshQuery* query, const dtMeshTile* tile, unsigned char flags, string & material);
	void NewCreateScene(Ogre::SceneNode * parent);
};



