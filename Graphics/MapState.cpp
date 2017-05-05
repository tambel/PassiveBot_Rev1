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
	//area->data_mutex.lock();
	UpdateScene();
	//CreateSceneTest();
	//MeshRenderable().createStaticMesh(false,mGraphicsSystem->getRoot(), mGraphicsSystem->getSceneManager());

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

void MapState::AddNavMesh(vector<rcPolyMesh*>& meshes)
{
	navigation_rends.push_back(new NavMeshRenderable(meshes));
}

void MapState::AddNavMesh(dtNavMesh * mesh, Vector3 offset)
{
	navigation_rends.push_back(new NavMeshRenderable(mesh, offset));
}

void MapState::AddLineStrip(vector<Vector3>& points)
{
	line_strip_rends.push_back(new LineStripRenderable(points));
}

void MapState::AddBoundingBox2D(BoundingBox & bb)
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
	AddLineStrip(points);
}

void MapState::AddMap()
{
	Ogre::ColourValue color;
	string material_name;
	Ogre::SceneManager * mSceneMgr = mGraphicsSystem->getSceneManager();

	vector<Renderable> er;
	int area_size = area->GetSize();
	bool exist = 0;
	bool kill = false;

	for (vector<Renderable>::iterator it = world_static_rends.begin(); it != world_static_rends.end();)
	{
		exist = false;
		for (auto &chunk : area->GetChunks())
		{
			if (it->GetModel() == &*chunk)
			{
				exist = true;
			}
		}
		for (auto &wmo : area->GetWMOs())
		{
			if (it->GetModel() == &*wmo)
			{
				exist = true;
			}

		}
		for (auto &doodad : area->GetDoodads())
		{
			if (it->GetModel() == &*doodad)
			{
				exist = true;
			}
		}
		if (!exist)
		{
			it = world_static_rends.erase(it);
		}
		else
		{
			++it;

		}
	}

	auto add_if_not_exist = [](Model * model, vector<Renderable> & rends, Ogre::SceneManager * mngr, string & material, Ogre::ColourValue & color)
	{
		bool exist = false;
		for (auto &rend : rends)
		{
			if (model == rend.GetModel())
			{
				return;
			}
		}
		rends.push_back(move(Renderable(model)));
		rends.back().CreateScene(mngr->getRootSceneNode(), material, color);
	};


	color = Ogre::ColourValue(1, 1, 0, 1);
	material_name = "Terrain";
	for (auto &chunk : area->GetChunks())
	{
		if (chunk->GetCoordinates() == Point2DI(0, 0))
			color = Ogre::ColourValue(1, 0, 0, 1);
		else if (chunk->GetCoordinates() == Point2DI(0, 15))
			color = Ogre::ColourValue(0, 1, 1, 0);
		else
			color = Ogre::ColourValue(1, 1, 0, 1);
		add_if_not_exist(&*chunk, world_static_rends, mSceneMgr, material_name, color);
	}
	color = Ogre::ColourValue(0, 1, 0, 1);
	material_name = "Wmo";
	for (auto &wmo : area->GetWMOs())
		add_if_not_exist(&*wmo, world_static_rends, mSceneMgr, material_name, color);
	color = Ogre::ColourValue(1, 0, 0, 1);
	material_name = "Doodad";
	for (auto &doodad : area->GetDoodads())
		add_if_not_exist(&*doodad, world_static_rends, mSceneMgr, material_name, color);
}



void MapState::UpdateScene()
{
	
	Ogre::ColourValue color;
	string material_name;
	Ogre::SceneManager * mSceneMgr = mGraphicsSystem->getSceneManager();
	AddMap();
	material_name = "NavMesh";
	
	for (auto &rend : navigation_rends)
	{
		rend->CreateScene(mSceneMgr->getRootSceneNode(), material_name, color);
	}


	for (auto & sr : line_strip_rends)
	{
		sr->CreateScene(mSceneMgr->getRootSceneNode(), string("Strip"), color);
	}
}