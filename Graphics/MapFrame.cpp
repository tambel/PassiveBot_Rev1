//#include "MapFrame.h"
//#include "WowDataLib\ADTWorker.h"
//#include <OgreSceneManager.h>
//
//#include "WowDataLib\SquareArea.h"
//#include "Renderable.h"
//#include <algorithm>
//#include "WowDataLib\Model.h"
//MapFrame::MapFrame(void)
//{
//	renderables=vector<Renderable*>();
//	doodads=vector<Renderable*>();
//	additional_objects=vector<Renderable*>();
//}
//
//
//MapFrame::~MapFrame(void)
//{
//
//}
//void MapFrame::createScene()
//{
//	area_scene=mSceneMgr->getRootSceneNode()->createChildSceneNode();
//	area_scene->setPosition(Ogre::Vector3(0,0,0));
//	//area_scene->rotate(Ogre::Vector3(0,0,1),Ogre::Radian(Ogre::Degree(90)),Ogre::Node::TS_LOCAL);
//	if (!area->busy)
//	{
//		area->busy=true;
//		InitTerrain();
//		if (area->to_update)
//		{
//			area->to_update=false;
//			InitDoodads();
//		}
//		mCamera->setPosition(Vector3ToOgreVector(area->GetChunks()[0][0]->GetRealPosition()));
//		area->busy=false;
//	}
//}
//
//void MapFrame::InitTerrain()
//{
//	for (auto rend:  renderables) rend->to_kill=true;
//	for (int i=0;i<area->GetRadius()*2+1;i++)
//	{
//		for (int j=0;j<area->GetRadius()*2+1;j++)
//		{
//			Chunk * chunk=area->GetChunks()[i][j];
//			if (chunk)
//			{
//				bool exist=false;
//				for (auto rend: renderables)
//				{
//					if (rend->GetModel()==chunk)
//					{
//						exist=true;
//						rend->to_kill=false;
//						break;
//					}
//				}
//				if (!exist)
//				{
//					Renderable * rend = new Renderable(chunk);
//					rend->CreateScene(mSceneMgr->getRootSceneNode());
//					//rend->GetScene()->setPosition(Vector3ToOgreVector(chunk->GetPosition()));
//					rend->GetScene()->setPosition(Vector3ToOgreVector(chunk->GetRealPosition()));
//					renderables.push_back(rend);
//				}
//
//			}
//		}
//	}
//	bool has_killed=false;
//	for (unsigned i=0;i<renderables.size();i++)
//	{
//		if (renderables[i]->to_kill)
//		{
//			has_killed=true;
//			delete renderables[i];
//			renderables[i]=0;
//		}
//	}
//	if (has_killed)
//		renderables.erase(remove(renderables.begin(), renderables.end(), (Renderable*)0), renderables.end());
//}
//void MapFrame::SetCamera(Utils::WowTypes::Position position)
//{
//	///mCamera->setPosition(Vector3ToOgreVector(position.coords));
//}
//void MapFrame::OnUpdate()
//{
//	if (!area->busy)
//	{
//		area->busy=true;
//		InitTerrain();
//		if (area->to_update)
//		{
//			area->to_update=false;
//			InitDoodads();
//		}
//		area->busy=false;
//	}
//	InitAdditionalObjects();
//
//}
//void MapFrame::InitDoodads()
//{
//	for (auto doodad:  doodads) 
//	{
//		doodad->to_kill=true;
//	}
//	for (auto &doodad:*area->GetActiveDoodas())
//	{
//
//		bool exist=false;
//		for (auto rend:doodads)
//		{
//
//			if (rend->GetModel()==doodad)
//			{
//				exist=true;
//				rend->to_kill=false;
//				break;
//			}
//		}
//		if (!exist)
//		{
//			Renderable * rend = new Renderable(doodad);
//			rend->CreateScene(mSceneMgr->getRootSceneNode());
//			rend->GetScene()->setPosition(Vector3ToOgreVector(doodad->GetPosition().coords));
//			//rend->GetScene()->setPosition(Ogre::Vector3(0,0,0));
//			doodads.push_back(rend);
//		}
//
//	}
//	bool has_killed=false;
//	for (unsigned i=0;i<doodads.size();i++)
//	{
//		if (doodads[i]->to_kill)
//		{
//			has_killed=true;
//			delete doodads[i];
//			doodads[i]=0;
//		}
//	}
//	if (has_killed)
//		doodads.erase(remove(doodads.begin(), doodads.end(), (Renderable*)0), doodads.end());
//}
//
//void MapFrame::InitAdditionalObjects()
//{
//	for (auto model:additional_objects)
//	{
//		model->to_kill=true;
//	}
//	for (auto avatar: *area->GetWowAvatars())
//	{
//		bool exist=false;
//		for (auto rend:additional_objects)
//		{
//
//			if (rend->GetModel()==avatar->doodad)
//			{
//				exist=true;
//				rend->to_kill=false;
//				rend->GetScene()->setPosition(Vector3ToOgreVector(avatar->GetPosition().coords));
//				break;
//			}
//		}
//		if (!exist)
//		{
//			Renderable * rend = new Renderable(avatar->doodad);
//			rend->CreateScene(mSceneMgr->getRootSceneNode());
//			rend->GetScene()->setPosition(Vector3ToOgreVector(avatar->GetPosition().coords));
//			//rend->GetScene()->setPosition(Ogre::Vector3(0,0,0));
//			additional_objects.push_back(rend);
//		}
//	}
//	bool has_killed=false;
//	for (unsigned i=0;i<additional_objects.size();i++)
//	{
//		if (additional_objects[i]->to_kill)
//		{
//			has_killed=true;
//			delete additional_objects[i];
//			additional_objects[i]=0;
//		}
//	}
//	if (has_killed)
//		additional_objects.erase(remove(additional_objects.begin(), additional_objects.end(), (Renderable*)0), additional_objects.end());
//}