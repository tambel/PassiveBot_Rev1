#pragma once
#include "Utils.h"

enum DrawNavMeshFlags
{
	DU_DRAWNAVMESH_OFFMESHCONS = 0x01,
	DU_DRAWNAVMESH_CLOSEDLIST = 0x02,
	DU_DRAWNAVMESH_COLOR_TILES = 0x04,
};
/*
enum dtPolyTypes
{
	/// The polygon is a standard convex polygon that is part of the surface of the mesh.
	DT_POLYTYPE_GROUND = 0,
	/// The polygon is an off-mesh connection consisting of two vertices.
	DT_POLYTYPE_OFFMESH_CONNECTION = 1,
};
*/



class Renderable
{
protected:
	Model * model = nullptr;
	static unsigned long counter;
	unsigned long id;
	Ogre::SceneNode * scene = nullptr;
	void _move(Renderable & other);
	Ogre::ManualObject * manual_object = nullptr;
public:
	bool to_kill;
	Renderable();
	Renderable(Model * model);



	Renderable(const Renderable &) = default;
	Renderable(Renderable &&);
	Renderable & operator=(const Renderable &) = default;
	Renderable & operator=(Renderable &&);

	~Renderable();
	virtual void CreateScene(Ogre::SceneNode * parent, string & material, Ogre::ColourValue & color = Ogre::ColourValue(1, 1, 0, 1));

	unsigned long GetID() { return id; }
	Ogre::SceneNode * GetScene() { return scene; }
	Model * GetModel() { return model; }

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
	virtual void CreateScene2(Ogre::SceneNode * parent);
	void CreateScene(Ogre::SceneNode * parent, string & material, Ogre::ColourValue & color);
};


class NavMeshRenderable : public Renderable
{
	vector<rcPolyMesh*> * meshes;
	dtNavMesh * mesh;
public:
	NavMeshRenderable(NavMeshRenderable && other);
	NavMeshRenderable(vector<rcPolyMesh*> & meshes);
	NavMeshRenderable(dtNavMesh * mesh);
	void DrawNavMesh(const dtNavMesh& mesh, const dtNavMeshQuery* query, unsigned char flags, string & material);
	void drawMeshTile(const dtNavMesh& mesh, const dtNavMeshQuery* query, const dtMeshTile* tile, unsigned char flags, string & material);
	void CreateScene(Ogre::SceneNode * parent, string & material, Ogre::ColourValue & color = Ogre::ColourValue(1, 1, 0, 1));
	void CreateScene2(Ogre::SceneNode * parent, string & material, Ogre::ColourValue & color = Ogre::ColourValue(1, 1, 0, 1));
};



