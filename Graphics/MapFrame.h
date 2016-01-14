#pragma once
#include "Window.h"
#include "WowWorldsData\SquareArea.h"
#include "WowInteraction\WowObject.h"
#include "Utils.h"
#include "Renderable.h"

class MapFrame: public Window
{
	SquareArea * area;
	Ogre::SceneNode * area_scene;
	vector<Renderable*> chunks;
	vector<Renderable*> doodads;
	vector<Renderable*> wmos;
	vector<Renderable*> additional_objects;
	Ogre::SceneNode * navmesh;
	rcPolyMesh  mesh;
	
public:
	PATHDATA * m_PathStore;
	MapFrame(void);
	~MapFrame(void);
	void createScene();
	void InitTerrain();
	void SetCamera(Utils::WowTypes::Position position);
	SquareArea * GetArea() {return area;}
	void SetArea(SquareArea * area) {this->area=area;}
	void OnUpdate();
	void InitDoodads();
	void InitWMOs();
	void InitAdditionalObjects();
	void SetNavMesh(const struct rcPolyMesh& mesh);
	void createNavMesh();
	void createRecastPathLine(int nPathSlot/*, PATHDATA * m_PathStore*/);
	void CreateChunks();
	void CreateNavMesh();
};

