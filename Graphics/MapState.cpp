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
	//AddPlayer(Doodad("D:\\Bot\\Extracted\\Character\\Tauren\\Male\\TaurenMale.M2", 0, Position(), 1024));

	//createRecastPathLine(0);
	//mCamera->setPosition(Vector3ToOgreVector(area->GetChunks()[0]->GetRealPosition()));
	//Vector3 & pos = area->GetChunks()[0]->GetRealPosition();
	//mCamera->setPosition(Ogre::Vector3(pos.x, pos.z, -pos.y));
	//mCamera->setOrientation(Ogre::Quaternion(Ogre::Vector3(0, 0, 0), Ogre::Vector3(0, 0, 0), Ogre::Vector3(0, 0, 1)));
	//mCamera->rotate(Ogre::Vector3(0, 0, 1), Ogre::Radian(Ogre::Degree(90)));
	//area->data_mutex.unlock();
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
	v = Ogre::Vector3(v.x, v.z, -v.y);

	mGraphicsSystem->getCamera()->setPosition(v);
	
	
	TutorialGameState::createScene01();
}

void MapState::AddNavMesh(vector<rcPolyMesh*>& meshes)
{
	navigation_rends.push_back(new NavMeshRenderable(meshes));
}

void MapState::UpdateScene()
{
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

	Ogre::SceneManager * mSceneMgr = mGraphicsSystem->getSceneManager();
	Ogre::ColourValue color = Ogre::ColourValue(1, 1, 0, 1);
	string material_name = "Terrain";
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
	material_name = "NavMesh";
	for (auto &rend : navigation_rends)
	{
		rend->CreateScene(mSceneMgr->getRootSceneNode(), material_name, color);
	}
}