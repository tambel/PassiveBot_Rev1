#pragma once
#include "Window.h"
//#include "WowWorldsData\NavArea.h"
#include "WowInteraction\WowObject.h"
#include "Utils.h"
#include "Renderable.h"
#include "MutableArea.h"
class MapFrame: public Window
{
	MutableArea * area;
	Ogre::SceneNode * area_scene;
	vector<Renderable*> additional_objects;
	Ogre::SceneNode * navmesh;
	rcPolyMesh  mesh;
	
public:
	//PATHDATA * m_PathStore;
	MapFrame(void);
	~MapFrame(void);
	void createScene();
	void SetCamera(Utils::WowTypes::Position position);
	Area * GetArea() {return area;}
	void SetArea(MutableArea * area) {this->area=area;}
	void OnUpdate();
	void InitAdditionalObjects();
	void createNavMesh();
	void createRecastPathLine(int nPathSlot/*, PATHDATA * m_PathStore*/);
	void CreateAreaScene();
	void CreateNavMesh();
};

