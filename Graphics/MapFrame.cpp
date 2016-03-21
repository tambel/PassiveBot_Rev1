#include "stdafx.h"
#include <algorithm>
#include "Recast.h"
#include "Sample.h"
#include "DetourNavMesh.h" 
#include "DetourNavMeshQuery.h" 
void MapFrame::AddPlayer(Model & model)
{
	player_renderable = AdditionalRenderable(&model);
	player_renderable.CreateScene(mSceneMgr->getRootSceneNode());
}
void MapFrame::_move(MapFrame & other)
{
	player_renderable = move(other.player_renderable);
	draw_paths_network = other.draw_paths_network;
	area = other.area;
	other.area = nullptr;
	ready = other.ready;
	rends = move(other.rends);
	map_points = move(other.map_points);
}
MapFrame::MapFrame(void)
{
	rends = vector<Renderable>();
	//frame_lock = shared_ptr<mutex>(new mutex());

}

MapFrame::MapFrame(MapFrame && other)
{
	_move(other);
}

MapFrame & MapFrame::operator=(MapFrame && other)
{
	_move(other);
	return *this;
}


MapFrame::~MapFrame(void)
{

}
void MapFrame::createScene()
{
	//DrawPathPoints();
	AddPlayer(Doodad("E:\\Extracted\\Character\\Tauren\\Male\\TaurenMale.M2", 0, Position(), 1024));
	DrawGlobalPaths();
	area->data_mutex.lock();
	
	UpdateScene();
	
	CreateNavMesh();

	//createRecastPathLine(0);
	mCamera->setPosition(Vector3ToOgreVector(area->GetChunks()[0]->GetRealPosition()));
	area->data_mutex.unlock();
	ready = true;

}
void MapFrame::WaitForReady()
{
	while (!ready)
		Sleep(1);
}
void MapFrame::OnUpdate()
{
	player_renderable.UpdatePosition();
	//if (to_update_player)
	{

		//player_scene->setPosition(Vector3ToOgreVector(player_position));

	//	to_update_player = false;
	}
	area->data_mutex.lock();
	if (area->to_update)
	{
		
		rends.clear();
		
		mSceneMgr->destroyAllManualObjects();
		mSceneMgr->destroyAllEntities();
		mSceneMgr->getRootSceneNode()->removeAndDestroyAllChildren();
		UpdateScene();
		DrawGlobalPaths();
		DrawPathPoints();
		AddPlayer(Doodad("E:\\Extracted\\Character\\Tauren\\Male\\TaurenMale.M2", 0, Position(), 1024));
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

	player_renderable.SetPosition(position,true);

	/*old_player_position = player_position;
	player_position = Vector3(Metrics::MapMidPoint - position.y, -(Metrics::MapMidPoint - position.x), position.z);
	if (old_player_position != player_position && !to_update_player)
	{
		to_update_player = true;
	}*/
	
	//player_position = position;
	

}

void MapFrame::DrawPathPoints()
{

	ifstream file;
	file.open("test.csv");
	MapPoint point;
	vector<MapPoint> points;
	while (!file.eof())
	{
		file.read(reinterpret_cast<char*>(&point),sizeof(MapPoint));
		points.push_back(point);
	}
	Doodad model=Doodad("E:\\Extracted\\Character\\Tauren\\Male\\TaurenMale.M2", 0, Position(), 1024);
	for (int i = 0; i < points.size()-1;i++)
	{
		MapPoint point = points[i];
		AdditionalRenderable rend = move(AdditionalRenderable(&model));
		rend.SetPosition(point.position, true);
		rend.CreateScene(mSceneMgr->getRootSceneNode());
		rend.UpdatePosition();
		map_points.push_back(move(rend));

	/*	Ogre::SceneNode * scene= mSceneMgr->getRootSceneNode()->createChildSceneNode();


		Renderable * rend = new Renderable(new Doodad("E:\\Extracted\\Character\\Tauren\\Male\\TaurenMale.M2", 0, Position(), 1024));
		
		Vector3 pos = Vector3(Metrics::MapMidPoint - point.position.y, -(Metrics::MapMidPoint - point.position.x), point.position.z);
		cout << pos.x << " " << pos.y << " " << pos.z << endl;
		
		scene->setPosition(Vector3ToOgreVector(pos));
		rend->CreateScene(scene);*/
		

	}
}
void MapFrame::DrawChunksLinks()
{
	struct MapPoint
	{
		Vector3 position;
		unsigned location_id;
		Point2D<int> block;
		Point2D<int> chunk;
	};

	struct Path
	{
		Vector3 points[500];
		unsigned points_number;
	};

	struct ChunksLink
	{
		MapPoint chunk1;
		MapPoint chunk2;
		Path path;

	};

	std::ifstream file;
	ChunksLink links[8];
	file.open("links.bin",ios::binary);
	file.read(reinterpret_cast<char*>(links), sizeof(ChunksLink) * 8);


}

void MapFrame::DrawGlobalPaths()
{
	struct MapPoint
	{
		Vector3 position;
		unsigned location_id;
		Point2D<int> block;
		Point2D<int> chunk;
	};

	struct Path
	{
		Vector3 points[500];
		unsigned points_number;
	};

	struct ChunksLink
	{
		MapPoint chunk1;
		MapPoint chunk2;
		Path path;

	};

	std::ifstream file;
	ChunksLink links[10];
	file.open("links.bin", ios::binary);
	file.read(reinterpret_cast<char*>(links), sizeof(ChunksLink) * 8);
	Ogre::SceneNode*      m_pRecastSN = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	
	for (int li = 0; li < 8; li++)
	{
		Ogre::ManualObject* m_pRecastMOPath = mSceneMgr->createManualObject("RecastMOPath"+to_string(li));
		m_pRecastMOPath->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
		Vector3 * path = links[li].path.points;
		//if (li==3)
		for (int i = 0; i < links[li].path.points_number; i ++)
		{
			m_pRecastMOPath->position(path[i].x, path[i].z, path[i].y + 2.0f);
			m_pRecastMOPath->colour(1, 0, 0);
		}
		m_pRecastMOPath->end();
		m_pRecastSN->attachObject(m_pRecastMOPath);
	}
	

}
