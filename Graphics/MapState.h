#pragma once
#include "Renderable.h"


using namespace Demo;

class MapState : public TutorialGameState
{
	Area * area;
	vector<Renderable> world_static_rends;
	vector<Renderable*> navigation_rends;
	void UpdateScene();
public:
	MapState(const Ogre::String &helpDescription);
	~MapState();
	virtual void createScene01(void);
	void SetArea(Area * area) { this->area = area; }
	//virtual void update(float timeSinceLast);
	
	void AddNavMesh(vector<rcPolyMesh*> & meshes);

};



