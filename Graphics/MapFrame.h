#pragma once
#include "Window.h"
#include "WowInteraction\WowObject.h"
#include "Utils.h"
#include "Renderable.h"
#include "MutableArea.h"
class MapFrame: public Window
{
	MutableArea * area;
	vector<Renderable> rends;

	
public:
	MapFrame(void);
	~MapFrame(void);
	void createScene();
	inline Area * GetArea() {return area;}
	inline void SetArea(MutableArea * area) {this->area=area;}
	void OnUpdate();
	void CreateNavMesh();
	void createRecastPathLine(int nPathSlot);
	void UpdateScene();
};

