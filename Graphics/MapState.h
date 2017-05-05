#pragma once
#include "Renderable.h"
#include <mutex>
using namespace Demo;

class MapState : public TutorialGameState
{
	Area * area;
	vector<Renderable> world_static_rends;
	vector<Renderable*> navigation_rends;
	vector<Renderable*> line_strip_rends;
	vector<Renderable*> rends;
	vector<Renderable*> new_rends;
	void UpdateScene();
	mutex state_mutex;
	bool is_new_rends_set = false;

public:
	MapState(const Ogre::String &helpDescription);
	~MapState();
	virtual void createScene01(void);
	void SetArea(Area * area) { this->area = area; }
	//virtual void update(float timeSinceLast);
	
	void AddNavMesh(vector<rcPolyMesh*> & meshes);
	void AddNavMesh(dtNavMesh * mesh, Vector3 offset=Vector3(0,0,0));
	void AddLineStrip(vector<Vector3> & points);
	void AddBoundingBox2D(BoundingBox & bb);
	void AddMap();
	vector<Renderable*> AddNavMesh2(dtNavMesh * mesh, Vector3 offset = Vector3(0, 0, 0));
	void SetNewRends();
	void CheckAndAddNewRends();

};



