#pragma once
#include "Window.h"
#include "Utils.h"
#include "Renderable.h"
#include "MutableArea.h"
class MapFrame: public Window
{
	MutableArea * area;
	vector<Renderable> rends;
	Renderable player_renderable;
	float m_smoothPath[2048 * 3];
	Vector3 player_position;
	Vector3 old_player_position;
	Ogre::SceneNode * player_scene = nullptr;
	bool to_update_player = false;
	vector<Renderable*> lines;
	

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
	void AddPlayer(Model & model);
	void SetPlayerPosition(Vector3 & position);
	void AddPath(Path::Link * link);
};

