#pragma once
#include "Window.h"
#include "WowInteraction\WowObject.h"
#include "Utils.h"
#include "Renderable.h"
#include "MutableArea.h"
#include "AdditionalRenderable.h"

struct MapPoint
{
	Vector3 position;
	unsigned location_id;
	Point2D<int> block;
	Point2D<int> chunk;
};

class MapFrame: public Window
{
	AdditionalRenderable player_renderable;
	bool draw_path_points = true;
	bool draw_paths_network = true;
	MutableArea * area;
	vector<Renderable> rends;
	//Renderable player_renderable;
	float m_smoothPath[2048 * 3];
	bool ready = false;
	vector<AdditionalRenderable> map_points;
	void _move(MapFrame & other);

public:
	MapFrame(void);
	MapFrame(MapFrame && other);
	MapFrame & operator=(MapFrame && other);
	~MapFrame(void);
	void createScene();
	inline Area * GetArea() {return area;}
	inline void SetArea(MutableArea * area) {this->area=area;}
	void WaitForReady();
	void OnUpdate();
	void CreateNavMesh();
	void createRecastPathLine(int nPathSlot);
	void UpdateScene();
	void AddPlayer(Model & model);
	void SetPlayerPosition(Vector3 & position);
	void DrawPathPoints();
	void DrawChunksLinks();
	void DrawGlobalPaths();
};

