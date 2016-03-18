#include "stdafx.h"
#include <algorithm>
#include "Recast.h"
#include "Sample.h"
#include "DetourNavMesh.h" 
#include "DetourNavMeshQuery.h" 
void MapFrame::AddPlayer(Model & model)
{
	player_renderable = move(Renderable(&model));
	player_scene = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	//player_position = Vector3(Metrics::MapMidPoint - player_position.y, -(Metrics::MapMidPoint - player_position.x), player_position.z);
	old_player_position = player_position;
	player_scene->setPosition(Vector3ToOgreVector(player_position));
	player_renderable.CreateScene(player_scene);

	

}
MapFrame::MapFrame(void)
{
	rends = vector<Renderable>();
}


MapFrame::~MapFrame(void)
{

}
void MapFrame::createScene()
{
	
	area->data_mutex.lock();
	UpdateScene();
	AddPlayer(Doodad("E:\\Extracted\\Character\\Tauren\\Male\\TaurenMale.M2", 0, Position(), 1024));
	CreateNavMesh();

	//createRecastPathLine(0);
	mCamera->setPosition(Vector3ToOgreVector(area->GetChunks()[0]->GetRealPosition()));
	area->data_mutex.unlock();


}
void MapFrame::OnUpdate()
{
	if (to_update_player)
	{
		player_scene->setPosition(Vector3ToOgreVector(player_position));
		to_update_player = false;
	}
	area->data_mutex.lock();
	if (area->to_update)
	{
		rends.clear();
		
		//mSceneMgr->destroyAllManualObjects();
		UpdateScene();
		
		CreateNavMesh();
		
		area->to_update = false;
	}
	area->data_mutex.unlock();
}



void MapFrame::createRecastPathLine(int nPathSlot/*, PATHDATA *m_PathStore*/)
{
	Ogre::SceneNode*      m_pRecastSN = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	Ogre::ManualObject* m_pRecastMOPath = mSceneMgr->createManualObject("RecastMOPath");
	m_pRecastMOPath->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
	for (int i = 0; i < area->m_nsmoothPath * 3; i += 3)
	{
		m_pRecastMOPath->position(area->m_smoothPath[i], area->m_smoothPath[i+2], area->m_smoothPath[i+1] + 2);
		m_pRecastMOPath->colour(1, 0, 0);
	}
	m_pRecastMOPath->end();
	m_pRecastSN->attachObject(m_pRecastMOPath);
}

void MapFrame::CreateNavMesh()
{
//	int count = 0;
//	Ogre::SceneNode * m_pRecastSN = mSceneMgr->getRootSceneNode()->createChildSceneNode();
//	for (auto &mmesh : area->polys)
//	{
//		rcPolyMesh & mesh = *mmesh.get();
//		const int nvp = mesh.nvp;
//		const float cs = mesh.cs;
//		const float ch = mesh.ch;
//		const float* orig = mesh.bmin;
//
//		int m_flDataX = mesh.npolys;
//		int m_flDataY = mesh.nverts;
//
//		// create scenenodes
//		int nIndex = 0;
//		int m_nAreaCount = mesh.npolys;
//		if (m_nAreaCount)
//		{
//
//			// start defining the manualObject
//			Ogre::ManualObject *m_pRecastMOWalk = mSceneMgr->createManualObject("RecastMOWalk" + to_string(count));
//			m_pRecastMOWalk->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
//			for (int i = 0; i < mesh.npolys; ++i) // go through all polygons
//				if (mesh.areas[i] == 0)
//				{
//					const unsigned short* p = &mesh.polys[i*nvp * 2];
//
//					unsigned short vi[3];
//					for (int j = 2; j < nvp; ++j) // go through all verts in the polygon
//					{
//						if (p[j] == RC_MESH_NULL_IDX) break;
//						vi[0] = p[0];
//						vi[1] = p[j - 1];
//						vi[2] = p[j];
//						for (int k = 0; k < 3; ++k) // create a 3-vert triangle for each 3 verts in the polygon.
//						{
//							const unsigned short* v = &mesh.verts[vi[k] * 3];
//							const float x = orig[0] + v[0] * cs;
//							const float y = orig[1] + (v[1] + 1)*ch;
//							const float z = orig[2] + v[2] * cs;
//
//							m_pRecastMOWalk->position(x, z, y);
//							if (mesh.areas[i] == 0)
//m_pRecastMOWalk->colour(0, 1, 0, 1);
//							else
//								m_pRecastMOWalk->colour(0, 1, 1, 1);
//
//						}
//						m_pRecastMOWalk->index(nIndex + 2);
//						m_pRecastMOWalk->index(nIndex + 1);
//						m_pRecastMOWalk->index(nIndex);
//						nIndex += 3;
//					}
//				}
//				m_pRecastMOWalk->end();
//
//
//				m_pRecastSN->attachObject(m_pRecastMOWalk);
//
//
//				Ogre::ManualObject*  m_pRecastMONeighbour = mSceneMgr->createManualObject("RecastMONeighbour" + to_string(count));
//				m_pRecastMONeighbour->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
//
//				for (int i = 0; i < mesh.npolys; ++i)
//				{
//					const unsigned short* p = &mesh.polys[i*nvp * 2];
//					for (int j = 0; j < nvp; ++j)
//					{
//						if (p[j] == RC_MESH_NULL_IDX) break;
//						if (p[nvp + j] == RC_MESH_NULL_IDX) continue;
//						int vi[2];
//						vi[0] = p[j];
//						if (j + 1 >= nvp || p[j + 1] == RC_MESH_NULL_IDX)
//							vi[1] = p[0];
//						else
//							vi[1] = p[j + 1];
//						for (int k = 0; k < 2; ++k)
//						{
//							const unsigned short* v = &mesh.verts[vi[k] * 3];
//							const float x = orig[0] + v[0] * cs;
//							const float y = orig[1] + (v[1] + 1)*ch + 0.1f;
//							const float z = orig[2] + v[2] * cs;
//							m_pRecastMONeighbour->position(x, z, y);
//							m_pRecastMONeighbour->colour(1, 1, 1);
//
//						}
//					}
//				}
//
//				m_pRecastMONeighbour->end();
//				m_pRecastSN->attachObject(m_pRecastMONeighbour);
//
//				Ogre::ManualObject * m_pRecastMOBoundary = mSceneMgr->createManualObject("RecastMOBoundary" + to_string(count));
//				m_pRecastMOBoundary->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
//
//				for (int i = 0; i < mesh.npolys; ++i)
//				{
//					const unsigned short* p = &mesh.polys[i*nvp * 2];
//					for (int j = 0; j < nvp; ++j)
//					{
//						if (p[j] == RC_MESH_NULL_IDX) break;
//						if (p[nvp + j] != RC_MESH_NULL_IDX) continue;
//						int vi[2];
//						vi[0] = p[j];
//						if (j + 1 >= nvp || p[j + 1] == RC_MESH_NULL_IDX)
//							vi[1] = p[0];
//						else
//							vi[1] = p[j + 1];
//						for (int k = 0; k < 2; ++k)
//						{
//							const unsigned short* v = &mesh.verts[vi[k] * 3];
//							const float x = orig[0] + v[0] * cs;
//							const float y = orig[1] + (v[1] + 1)*ch + 0.1f;
//							const float z = orig[2] + v[2] * cs;
//							//dd->vertex(x, y, z, colb);
//
//							m_pRecastMOBoundary->position(x, z + 0.25f, y);
//							m_pRecastMOBoundary->colour(0, 0, 0);
//						}
//					}
//				}
//
//				m_pRecastMOBoundary->end();
//				m_pRecastSN->attachObject(m_pRecastMOBoundary);
//		}
//		count++;
//	}
}

void MapFrame::UpdateScene()
{
	vector<Renderable> er;
	int area_size = area->GetSize();
	bool exist = 0;
	bool kill = false;

	for (vector<Renderable>::iterator it = rends.begin(); it != rends.end();)
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
			it = rends.erase(it);
		}
		else
		{
			++it;

		}
	}

	auto add_if_not_exist = [](Model * model, vector<Renderable> & rends, Ogre::SceneManager * mngr)
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
		rends.back().CreateScene(mngr->getRootSceneNode());
	};
	for (auto &chunk : area->GetChunks())
	{
		add_if_not_exist(&*chunk, rends, mSceneMgr);
	}
	for (auto &wmo : area->GetWMOs())
		add_if_not_exist(&*wmo, rends, mSceneMgr);
	for (auto &doodad : area->GetDoodads())
		add_if_not_exist(&*doodad, rends, mSceneMgr);
}

void MapFrame::SetPlayerPosition(Vector3 & position)
{
	old_player_position = player_position;
	player_position = Vector3(Metrics::MapMidPoint - position.y, -(Metrics::MapMidPoint - position.x), position.z);
	if (old_player_position.To2D() != player_position.To2D() && !to_update_player)
	{
		to_update_player = true;
	}
	
	//player_position = position;
	

}