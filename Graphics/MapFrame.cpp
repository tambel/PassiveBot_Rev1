#include "stdafx.h"
#include <algorithm>
#include "Recast.h"
#include "Sample.h"
#include "DetourNavMesh.h" 
#include "DetourNavMeshQuery.h" 
MapFrame::MapFrame(void)
{

	additional_objects = vector<Renderable*>();
}


MapFrame::~MapFrame(void)
{

}
void MapFrame::createScene()
{
	area->data_mutex.lock();
	CreateAreaScene();
	CreateNavMesh();
	//createRecastPathLine(0);
	for (int i = 0; i < area->GetRadius() * 2 + 1; i++)
	{
		bool br = false;
		for (int j = 0; j < area->GetRadius() * 2 + 1; j++)
		{
			if (area->GetChunks()[i][j])
			{
				mCamera->setPosition(Vector3ToOgreVector(area->GetChunks()[i][j]->GetRealPosition()));
				break;
			}
		}
		if (br) break;

	}
	//mCamera->setPosition(0,0,0);
	area->data_mutex.unlock();
	//createNavMesh();
	//createRecastPathLine(0);


}

void MapFrame::SetCamera(Utils::WowTypes::Position position)
{
	///mCamera->setPosition(Vector3ToOgreVector(position.coords));
}
void MapFrame::OnUpdate()
{
	area->data_mutex.lock();
	if (area->to_update)
	{
		mSceneMgr->destroyAllManualObjects();
		CreateAreaScene();
		CreateNavMesh();
		area->to_update = false;
	}
	area->data_mutex.unlock();
	//InitAdditionalObjects();

}

void MapFrame::InitAdditionalObjects()
{
	for (auto model : additional_objects)
	{
		model->to_kill = true;
	}
	for (auto avatar : *area->GetWowAvatars())
	{
		bool exist = false;
		for (auto rend : additional_objects)
		{

			if (rend->GetModel() == avatar->doodad)
			{
				exist = true;
				rend->to_kill = false;
				rend->GetScene()->setPosition(Vector3ToOgreVector(avatar->GetPosition().coords));
				break;
			}
		}
		if (!exist)
		{
			Renderable * rend = new Renderable(avatar->doodad);
			rend->CreateScene(mSceneMgr->getRootSceneNode());
			rend->GetScene()->setPosition(Vector3ToOgreVector(avatar->GetPosition().coords));
			//rend->GetScene()->setPosition(Ogre::Vector3(0,0,0));
			additional_objects.push_back(rend);
		}
	}
	bool has_killed = false;
	for (unsigned i = 0; i < additional_objects.size(); i++)
	{
		if (additional_objects[i]->to_kill)
		{
			has_killed = true;
			delete additional_objects[i];
			additional_objects[i] = 0;
		}
	}
	if (has_killed)
		additional_objects.erase(remove(additional_objects.begin(), additional_objects.end(), (Renderable*)0), additional_objects.end());
}


void MapFrame::createRecastPathLine(int nPathSlot/*, PATHDATA *m_PathStore*/)
{
	//PATHDATA * m_PathStore = area->m_PathStore.get();
	//if (!m_PathStore) return;
	Ogre::SceneNode*      m_pRecastSN = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	Ogre::ManualObject* m_pRecastMOPath = mSceneMgr->createManualObject("RecastMOPath");
	m_pRecastMOPath->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);


	/*int nVertCount = m_PathStore->MaxVertex;
	for (int nVert = 0; nVert<nVertCount; nVert++)
	{
		m_pRecastMOPath->position(m_PathStore->PosX[nVert], m_PathStore->PosZ[nVert], m_PathStore->PosY[nVert]+2);
		m_pRecastMOPath->colour(1, 0, 0);
	}*/
	for (int i = 0; i < area->m_nsmoothPath * 3; i += 3)
	{
		m_pRecastMOPath->position(area->m_smoothPath[i], area->m_smoothPath[i+2], area->m_smoothPath[i+1] + 2);
		m_pRecastMOPath->colour(1, 0, 0);
	}




	m_pRecastMOPath->end();
	m_pRecastSN->attachObject(m_pRecastMOPath);
}

void MapFrame::CreateAreaScene()
{

	int area_size = area->GetSize();
	Ogre::SceneNode * area_scene = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	Ogre::ManualObject * man = mSceneMgr->createManualObject();
	man->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	unsigned vert_offset = 0;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			auto createObject = [](Ogre::ManualObject * man, float * vertices, unsigned vert_count, int *indices, unsigned ind_count, unsigned  & vert_offset )
			{ 
				for (int vi = 0; vi < vert_count*3; vi += 3)
				{
					man->position(vertices[vi], vertices[vi + 2], vertices[vi + 1]);
					if (vi % 9 == 0)
						man->colour(1, 1, 1, 1);
					else if (vi % 6 == 0)
						man->colour(0, 0, 0, 0);
					else
						man->colour(1, 1, 0, 1);
				}
				for (int ii = 0; ii < ind_count; ii += 3)
				{
					man->index(indices[ii+2] + vert_offset);
					man->index(indices[ii + 1] + vert_offset);
					man->index(indices[ii] + vert_offset);
				}
				vert_offset += vert_count;
			};
			Chunk *chunk = area->GetChunks()[i][j];
			if (!chunk)
				continue;
			createObject(man,chunk->rvertices, chunk->GetVertexCount(), chunk->GetIndices(), chunk->GetIndexCount(), vert_offset);
			for (auto doodad : chunk->GetDoodads())
			{
				createObject(man, doodad->rvertices, doodad->GetVertexCount(), doodad->GetIndices(), doodad->GetIndexCount(), vert_offset);
			}
			for (auto wmo : chunk->GetWMOs())
			{
				//for (auto &part : wmo->GetParts())
				{
					createObject(man, wmo->rvertices, wmo->GetVertexCount(), wmo->GetIndices(), wmo->GetIndexCount(), vert_offset);
				}
			}
			/*float * vertices = chunk->nav_vertices.get();
			int * indices = chunk->nav_indices.get();
			int vert_count = chunk->vert_count * 3;*/
			
			

		}
	}
	man->end();
	area_scene->attachObject(man);
}

void MapFrame::CreateNavMesh()
{
	int count = 0;
	Ogre::SceneNode * m_pRecastSN = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	for (auto &mmesh : area->polys)
	{
		rcPolyMesh & mesh = *mmesh.get();
		const int nvp = mesh.nvp;
		const float cs = mesh.cs;
		const float ch = mesh.ch;
		const float* orig = mesh.bmin;

		int m_flDataX = mesh.npolys;
		int m_flDataY = mesh.nverts;

		// create scenenodes
		int nIndex = 0;
		int m_nAreaCount = mesh.npolys;
		if (m_nAreaCount)
		{

			// start defining the manualObject
			Ogre::ManualObject *m_pRecastMOWalk = mSceneMgr->createManualObject("RecastMOWalk" + to_string(count));
			m_pRecastMOWalk->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
			for (int i = 0; i < mesh.npolys; ++i) // go through all polygons
				if (mesh.areas[i] == 0)
				{
					const unsigned short* p = &mesh.polys[i*nvp * 2];

					unsigned short vi[3];
					for (int j = 2; j < nvp; ++j) // go through all verts in the polygon
					{
						if (p[j] == RC_MESH_NULL_IDX) break;
						vi[0] = p[0];
						vi[1] = p[j - 1];
						vi[2] = p[j];
						for (int k = 0; k < 3; ++k) // create a 3-vert triangle for each 3 verts in the polygon.
						{
							const unsigned short* v = &mesh.verts[vi[k] * 3];
							const float x = orig[0] + v[0] * cs;
							const float y = orig[1] + (v[1] + 1)*ch;
							const float z = orig[2] + v[2] * cs;

							m_pRecastMOWalk->position(x, z, y);
							if (mesh.areas[i] == 0)
								m_pRecastMOWalk->colour(0, 1, 0, 1);
							else
								m_pRecastMOWalk->colour(0, 1, 1, 1);

						}
						m_pRecastMOWalk->index(nIndex + 2);
						m_pRecastMOWalk->index(nIndex + 1);
						m_pRecastMOWalk->index(nIndex);
						nIndex += 3;
					}
				}
			m_pRecastMOWalk->end();


			m_pRecastSN->attachObject(m_pRecastMOWalk);


			Ogre::ManualObject*  m_pRecastMONeighbour = mSceneMgr->createManualObject("RecastMONeighbour" + to_string(count));
			m_pRecastMONeighbour->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);

			for (int i = 0; i < mesh.npolys; ++i)
			{
				const unsigned short* p = &mesh.polys[i*nvp * 2];
				for (int j = 0; j < nvp; ++j)
				{
					if (p[j] == RC_MESH_NULL_IDX) break;
					if (p[nvp + j] == RC_MESH_NULL_IDX) continue;
					int vi[2];
					vi[0] = p[j];
					if (j + 1 >= nvp || p[j + 1] == RC_MESH_NULL_IDX)
						vi[1] = p[0];
					else
						vi[1] = p[j + 1];
					for (int k = 0; k < 2; ++k)
					{
						const unsigned short* v = &mesh.verts[vi[k] * 3];
						const float x = orig[0] + v[0] * cs;
						const float y = orig[1] + (v[1] + 1)*ch + 0.1f;
						const float z = orig[2] + v[2] * cs;
						m_pRecastMONeighbour->position(x, z, y);
						m_pRecastMONeighbour->colour(1, 1, 1);

					}
				}
			}

			m_pRecastMONeighbour->end();
			m_pRecastSN->attachObject(m_pRecastMONeighbour);

			Ogre::ManualObject * m_pRecastMOBoundary = mSceneMgr->createManualObject("RecastMOBoundary"+to_string(count));
			m_pRecastMOBoundary->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);

			for (int i = 0; i < mesh.npolys; ++i)
			{
				const unsigned short* p = &mesh.polys[i*nvp * 2];
				for (int j = 0; j < nvp; ++j)
				{
					if (p[j] == RC_MESH_NULL_IDX) break;
					if (p[nvp + j] != RC_MESH_NULL_IDX) continue;
					int vi[2];
					vi[0] = p[j];
					if (j + 1 >= nvp || p[j + 1] == RC_MESH_NULL_IDX)
						vi[1] = p[0];
					else
						vi[1] = p[j + 1];
					for (int k = 0; k < 2; ++k)
					{
						const unsigned short* v = &mesh.verts[vi[k] * 3];
						const float x = orig[0] + v[0] * cs;
						const float y = orig[1] + (v[1] + 1)*ch + 0.1f;
						const float z = orig[2] + v[2] * cs;
						//dd->vertex(x, y, z, colb);

						m_pRecastMOBoundary->position(x, z + 0.25, y);
						m_pRecastMOBoundary->colour(0, 0, 0);
					}
				}
			}

			m_pRecastMOBoundary->end();
			m_pRecastSN->attachObject(m_pRecastMOBoundary);
		}
		count++;
	}
}
