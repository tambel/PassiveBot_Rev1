#include "stdafx.h"
#include "MapState.h"

#include "OgreEntity.h"
#include "OgreMesh.h"

#include "OgreRoot.h"
#include "OgreHlmsManager.h"

#include "OgreCamera.h"


MapState::MapState(const Ogre::String & helpDescription) :
	TutorialGameState(helpDescription)
{
}

MapState::~MapState()
{
}

void MapState::createScene01(void)
{
	AddMap();
	mCameraController = new CameraController(mGraphicsSystem, false);
	
	Ogre::Light *light = mGraphicsSystem->getSceneManager()->createLight();
	Ogre::SceneNode *lightNode = mGraphicsSystem->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(light);

	Ogre::Vector3 v = Vector3ToOgreVector(area->GetCenter());
	lightNode->setPosition(v.x,v.y+1000,-v.z);

	light->setPowerScale(Ogre::Math::PI); //Since we don't do HDR, counter the PBS' division by PI
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(Ogre::Vector3(0, -1,-1).normalisedCopy());
	light->setAttenuation(1100, 1, 100, 1);
	v = Vector3ToOgreVector( area->GetChunks()[0]->GetRealPosition());
	//v = Vector3ToOgreVector(area->GetBoundingBox().up);
	v = Ogre::Vector3(v.x, v.z, -v.y);

	mGraphicsSystem->getCamera()->setPosition(v);
	mGraphicsSystem->getCamera()->setFarClipDistance(0);
	mGraphicsSystem->getCamera()->setCastShadows(false);
	//mGraphicsSystem->getRoot()->getHlmsManager()->getDatablock("Terrain")->mShadowConstantBias=25.0;

	TutorialGameState::createScene01();
}

void MapState::OldAddNavMesh(vector<rcPolyMesh*>& meshes)
{
	navigation_rends.push_back(new NavMeshRenderable(meshes));
}

void MapState::OldAddNavMesh(dtNavMesh * mesh, Vector3 offset)
{
	navigation_rends.push_back(new NavMeshRenderable(mesh, offset));
}

vector<Renderable*> MapState::AddNavMesh(dtNavMesh * mesh, Vector3 offset)
{
	NavMeshRenderable * rend = new NavMeshRenderable(mesh, offset);
	//NavMeshRenderable r = move(NavMeshRenderable(mesh, offset));
	new_rends.push_back(rend);
	SetNewRends();
	return vector<Renderable*>({ rend });
	
}

inline void MapState::SetNewRends()
{
	state_mutex.lock();
	is_new_rends_set = true;
	state_mutex.unlock();
}

void MapState::CheckAndAddNewRends()
{
	auto root_scene = mGraphicsSystem->getSceneManager()->getRootSceneNode();
	state_mutex.lock();
	if (is_new_rends_set)
	{
		for (auto rend : new_rends)
		{
			rend->NewCreateScene(root_scene);
		}
		rends.insert(rends.end(), new_rends.begin(), new_rends.end());
		new_rends.clear();
		is_new_rends_set = false;
	}
	state_mutex.unlock();
}

vector<Renderable*> MapState::AddLineStrip(vector<Vector3>& points)
{
	Renderable * rend = new LineStripRenderable(points);
	new_rends.push_back(new LineStripRenderable(points));
	SetNewRends();
	return vector<Renderable*>({ rend });
}

vector<Renderable*> MapState::AddBoundingBox2D(BoundingBox & bb)
{
	vector<Vector3> points;
	points.push_back(bb.up);
	points.push_back(Vector3(bb.down.x, bb.up.y, bb.up.z));
	points.push_back(Vector3(bb.down.x, bb.up.y, bb.up.z));
	points.push_back(bb.down);
	points.push_back(Vector3(bb.up.x, bb.up.y, bb.down.z));
	points.push_back(Vector3(bb.up.x, bb.up.y, bb.down.z));
	points.push_back(bb.up);
	points.push_back(bb.up);
	
	//points.push_back(bb.up);
	return move(AddLineStrip(points));
}

vector<Renderable*> MapState::AddMap()
{
	vector<Renderable*> rrends;
	Ogre::ColourValue color;
	string material_name;
	Ogre::SceneManager * mSceneMgr = mGraphicsSystem->getSceneManager();
	int area_size = area->GetSize();
	Renderable * rend;
	auto add_rend = [&rrends,&rend, this](Model * model, string & material)
	{
		rend = new WowModelRenderable(model, material);
		new_rends.push_back(rend);
		rrends.push_back(rend);
	};
	string material = "Terrain";
	for (auto &chunk : area->GetChunks())add_rend(&*chunk, material);
	material = "Doodad";
	for (auto &doodad : area->GetDoodads())add_rend(&*doodad, material);
	material = "Wmo";
	for (auto &wmo : area->GetWMOs())add_rend(&*wmo, material);
	SetNewRends();
	return rrends;
}





void MapState::UpdateScene()
{
	
}