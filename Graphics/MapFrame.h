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
public:
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
};

