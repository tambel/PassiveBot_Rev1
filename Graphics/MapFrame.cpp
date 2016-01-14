#include "stdafx.h"
#include <algorithm>
#include "Recast.h"
#include "Sample.h"
#include "DetourNavMesh.h" 
#include "DetourNavMeshQuery.h" 
MapFrame::MapFrame(void)
{
	chunks = vector<Renderable*>();
	doodads = vector<Renderable*>();
	wmos = vector<Renderable*>();
	additional_objects = vector<Renderable*>();
}


MapFrame::~MapFrame(void)
{

}
void MapFrame::createScene()
{
	

	//area_scene = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	
	//area_scene->setPosition(Ogre::Vector3(0, 0, 0));
	//area_scene->rotate(Ogre::Vector3(0,0,1),Ogre::Radian(Ogre::Degree(90)),Ogre::Node::TS_LOCAL);
	if (!area->busy)
	{
		area->busy = true;
		CreateChunks();
		CreateNavMesh();
		//InitTerrain();
		if (area->to_update)
		{
			area->to_update = false;
			//InitDoodads();
			//InitWMOs();
		}
		for (int i = 0; i < area->GetRadius() * 2 + 1; i++)
		{
			bool br = false;
			for (int j = 0; j < area->GetRadius() * 2 + 1; j++)
			{
				if (area->GetChunks()[i][j])
				{
					mCamera->setPosition(Vector3ToOgreVector(area->GetChunks()[0][0]->GetRealPosition()));
					break;
				}
			}
			if (br) break;

		}
		//mCamera->setPosition(0,0,0);
		area->busy = false;
		//createNavMesh();
		//createRecastPathLine(0);
	}

	
}

void MapFrame::InitTerrain()
{

	for (auto rend : chunks) rend->to_kill = true;
	for (int i = 0; i < area->GetRadius() * 2 + 1; i++)
	{
		for (int j = 0; j < area->GetRadius() * 2 + 1; j++)
		{
			Chunk * chunk = area->GetChunks()[i][j];
			if (chunk)
			{
				bool exist = false;
				for (auto rend : chunks)
				{
					if (rend->GetModel() == chunk)
					{
						exist = true;
						rend->to_kill = false;
						break;
					}
				}
				if (!exist)
				{
					Renderable * rend = new Renderable(chunk);
					rend->CreateScene(mSceneMgr->getRootSceneNode());
					//rend->GetScene()->setPosition(Vector3ToOgreVector(chunk->GetGamePosition()));
					rend->GetScene()->setPosition(Vector3ToOgreVector(chunk->GetRealPosition()));
					chunks.push_back(rend);
				}
				Ogre::SceneNode * m_pRecastSN;
				if (chunk->result_mesh)
				{
					rcPolyMesh & mesh = *chunk->result_mesh;
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
						Ogre::ManualObject *m_pRecastMOWalk = mSceneMgr->createManualObject("RecastMOWalk"+to_string(chunk->GetCoordinates().X)+to_string(chunk->GetCoordinates().Y));
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
									//m_pRecastMOWalk->triangle(nIndex, nIndex + 1, nIndex + 2);
									m_pRecastMOWalk->index(nIndex + 2);
									m_pRecastMOWalk->index(nIndex + 1);
									m_pRecastMOWalk->index(nIndex);
									nIndex += 3;
								}
							}
						m_pRecastMOWalk->end();

						m_pRecastSN = area_scene->createChildSceneNode();
						//m_pRecastSN->attachObject(m_pRecastMOWalk);
						m_pRecastSN->setPosition(Vector3ToOgreVector(chunk->GetRealPosition()));
						//m_pRecastSN->setPosition(Vector3ToOgreVector(area->GetBoundingBox().up));
						//m_pRecastSN->setPosition(m_pRecastSN->getPosition().x, m_pRecastSN->getPosition().y, m_pRecastSN->getPosition().z);
					}
				}
			}
		}
	}
	bool has_killed = false;
	for (unsigned i = 0; i < chunks.size(); i++)
	{
		if (chunks[i]->to_kill)
		{
			has_killed = true;
			delete chunks[i];
			chunks[i] = 0;
		}
	}
	if (has_killed)
		chunks.erase(remove(chunks.begin(), chunks.end(), (Renderable*)0), chunks.end());
}
void MapFrame::SetCamera(Utils::WowTypes::Position position)
{
	///mCamera->setPosition(Vector3ToOgreVector(position.coords));
}
void MapFrame::OnUpdate()
{
	if (!area->busy)
	{
		area->busy = true;
		
		if (area->to_update)
		{
			InitTerrain();
			area->to_update = false;
			InitDoodads();
			InitWMOs();
			//createNavMesh();
			//createRecastPathLine(0);

		}

		area->busy = false;
	}
	InitAdditionalObjects();

}
void MapFrame::InitDoodads()
{
	for (auto doodad : doodads)
	{
		doodad->to_kill = true;
	}
	for (auto &doodad : *area->GetActiveDoodas())
	{

		bool exist = false;
		for (auto rend : doodads)
		{

			if (rend->GetModel() == doodad)
			{
				exist = true;
				rend->to_kill = false;
				break;
			}
		}
		if (!exist)
		{
			Renderable * rend = new Renderable(doodad);
			if (!rend->CreateScene(mSceneMgr->getRootSceneNode()))
				continue;
			rend->GetScene()->setPosition(Vector3ToOgreVector(doodad->GetPosition().coords));
			//rend->GetScene()->setPosition(Ogre::Vector3(0,0,0));
			doodads.push_back(rend);
		}

	}
	bool has_killed = false;
	for (unsigned i = 0; i < doodads.size(); i++)
	{
		if (doodads[i]->to_kill)
		{
			has_killed = true;
			delete doodads[i];
			doodads[i] = 0;
		}
	}
	if (has_killed)
		doodads.erase(remove(doodads.begin(), doodads.end(), (Renderable*)0), doodads.end());
}

void MapFrame::InitWMOs()
{
	for (auto wmo : wmos)
	{
		wmo->to_kill = true;
	}
	for (auto &wmo : area->GetActiveWMOs())
	{
		if (!wmo)
			continue;
		for (auto &part : wmo->GetParts())
		{
			//if (part)
			{
				bool exist = false;
				for (auto rend : wmos)
				{

					if (rend->GetModel() == &part)
					{
						exist = true;
						rend->to_kill = false;
						break;
					}
				}
				if (!exist)
				{
					
					Renderable * rend = new Renderable(&part);
					if (!rend->CreateScene(mSceneMgr->getRootSceneNode()))
						continue;
					rend->GetScene()->setPosition(Vector3ToOgreVector(wmo->GetPosition().coords));
					//rend->GetScene()->setPosition(Ogre::Vector3(0,0,0));
					wmos.push_back(rend);
				}
			}
		}
	}
	bool has_killed = false;
	for (unsigned i = 0; i < wmos.size(); i++)
	{
		if (wmos[i]->to_kill)
		{
			has_killed = true;
			delete wmos[i];
			wmos[i] = 0;
		}
	}
	if (has_killed)
		wmos.erase(remove(wmos.begin(), wmos.end(), (Renderable*)0), wmos.end());
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

void MapFrame::SetNavMesh(const struct rcPolyMesh& mesh)
{
	this->mesh = mesh;

}

void MapFrame::createNavMesh()
{
	
	Ogre::SceneNode * m_pRecastSN;
	rcPolyMesh & mesh=*area->Navigation().result_mesh;
	const int nvp = mesh.nvp;
	const float cs = mesh.cs;
	const float ch = mesh.ch;
	const float* orig = mesh.bmin;

	int m_flDataX = mesh.npolys;
	int m_flDataY = mesh.nverts;

	// create scenenodes
	try
	{
		m_pRecastSN = mSceneMgr->getSceneNode("RecastSN");
		Ogre::SceneNode::ObjectIterator iter = m_pRecastSN->getAttachedObjectIterator();
		mSceneMgr->destroyAllManualObjects();
	}
	catch (Ogre::Exception e)
	{
		m_pRecastSN = mSceneMgr->getRootSceneNode()->createChildSceneNode("RecastSN");
	}

	int nIndex = 0;
	int m_nAreaCount = mesh.npolys;


	if (m_nAreaCount)
	{

		// start defining the manualObject
		Ogre::ManualObject *m_pRecastMOWalk = mSceneMgr->createManualObject("RecastMOWalk");
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
							m_pRecastMOWalk->colour(0, 1, 1,1);

					}
					//m_pRecastMOWalk->triangle(nIndex, nIndex + 1, nIndex + 2);
					m_pRecastMOWalk->index(nIndex+2);
					m_pRecastMOWalk->index(nIndex+1);
					m_pRecastMOWalk->index(nIndex);
					nIndex += 3;
				}
			}
		m_pRecastMOWalk->end();
		
		m_pRecastSN = area_scene->createChildSceneNode();
		m_pRecastSN->attachObject(m_pRecastMOWalk);
		m_pRecastSN->setPosition(Vector3ToOgreVector( area->GetBoundingBox().up));
		m_pRecastSN->setPosition(m_pRecastSN->getPosition().x, m_pRecastSN->getPosition().y, m_pRecastSN->getPosition().z);



		Ogre::ManualObject*  m_pRecastMONeighbour = mSceneMgr->createManualObject("RecastMONeighbour");
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
				if (j + 1 >= nvp || p[j + 1] == RC_MESH_NULL_IDX)http://lurkmore.to/%D0%A4%D0%93%D0%9C#
					vi[1] = p[0];
				else
					vi[1] = p[j + 1];
				for (int k = 0; k < 2; ++k)
				{
					const unsigned short* v = &mesh.verts[vi[k] * 3];
					const float x = orig[0] + v[0] * cs;
					const float y = orig[1] + (v[1] + 1)*ch + 0.1f;
					const float z = orig[2] + v[2] * cs;
					//dd->vertex(x, y, z, coln);
					m_pRecastMONeighbour->position(x, z, y);
					m_pRecastMONeighbour->colour(1, 1, 1);

				}
			}
		}

		m_pRecastMONeighbour->end();
		m_pRecastSN->attachObject(m_pRecastMONeighbour);
		/*Ogre::SceneNode * */m_pRecastSN = area_scene->createChildSceneNode();
		//m_pRecastSN->attachObject(m_pRecastMONeighbour);
		//m_pRecastSN->setPosition(Vector3ToOgreVector(area->GetBoundingBox().up));

		Ogre::ManualObject * m_pRecastMOBoundary = mSceneMgr->createManualObject("RecastMOBoundary");
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

					m_pRecastMOBoundary->position(x, z +0.25, y);
					m_pRecastMOBoundary->colour(0, 0, 0);
				}
			}
		}

		m_pRecastMOBoundary->end();
		m_pRecastSN->attachObject(m_pRecastMOBoundary);
		//Ogre::SceneNode * s2 = area_scene->createChildSceneNode();
		//m_pRecastSN->attachObject(m_pRecastMOBoundary);
		m_pRecastSN->setPosition(Vector3ToOgreVector(area->GetBoundingBox().up));

	}
}

void MapFrame::createRecastPathLine(int nPathSlot/*, PATHDATA *m_PathStore*/)
{
	PATHDATA * m_PathStore = area->m_PathStore;
	Ogre::ManualObject* m_pRecastMOPath;
	Ogre::SceneNode*      m_pRecastSN = area_scene->createChildSceneNode();
	/*if (m_pRecastMOPath)
	{
		m_pRecastSN->detachObject("RecastMOPath");
		mSceneMgr->destroyManualObject(m_pRecastMOPath);
		m_pRecastMOPath = NULL;
	}*/


	m_pRecastMOPath = mSceneMgr->createManualObject("RecastMOPath");
	m_pRecastMOPath->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);


	int nVertCount = m_PathStore[nPathSlot].MaxVertex;
	for (int nVert = 0; nVert<nVertCount; nVert++)
	{
		m_pRecastMOPath->position(m_PathStore[nPathSlot].PosX[nVert], m_PathStore[nPathSlot].PosZ[nVert], m_PathStore[nPathSlot].PosY[nVert]+2);
		m_pRecastMOPath->colour(1, 0, 0);

		//sprintf(m_chBug, "Line Vert %i, %f %f %f", nVert, m_PathStore[nPathSlot].PosX[nVert], m_PathStore[nPathSlot].PosY[nVert], m_PathStore[nPathSlot].PosZ[nVert]) ;
		//m_pLog->logMessage(m_chBug);
	}




	m_pRecastMOPath->end();
	m_pRecastSN->attachObject(m_pRecastMOPath);
	//m_pRecastSN->setPosition(Vector3ToOgreVector(area->GetBoundingBox().up));
	//m_pRecastSN->setPosition(m_pRecastSN->getPosition().x, m_pRecastSN->getPosition().z, m_pRecastSN->getPosition().y);
}

void MapFrame::CreateChunks()
{

	int area_size = area->GetSize();
	Ogre::SceneNode * area_scene = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	Ogre::ManualObject * man = mSceneMgr->createManualObject();
	man->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	int vert_offset = 0;
	for (int i = 0; i < area_size; i++)
	{
		for (int j = 0; j < area_size; j++)
		{
			Chunk *chunk = area->GetChunks()[i][j];
			if (!chunk)
				continue;
			float * vertices = chunk->nav_vertices.get();
			int * indices = chunk->nav_indices.get();
			int vert_count = chunk->vert_count * 3;
			
			for (int vi = 0; vi < vert_count; vi += 3)
			{
				man->position(vertices[vi], vertices[vi + 2], vertices[vi + 1]);
				if (vi%9==0)
					man->colour(1, 1, 1, 1);
				else if (vi%6==0)
					man->colour(0, 0, 0, 0);
				else 
					man->colour(1, 1, 0, 1);
			}
			for (int ii = 0; ii < chunk->ind_count; ii+=3)
			{
				man->index(indices[ii+2]+ vert_offset);
				man->index(indices[ii + 1] + vert_offset);
				man->index(indices[ii ] + vert_offset);
			}
			vert_offset += chunk->vert_count;

		}
	}
	man->end();
	area_scene->attachObject(man);
	mCamera->setPosition(Vector3ToOgreVector( area->GetChunks()[0][0]->GetRealPosition()));
}

void MapFrame::CreateNavMesh()
{
	int count = 0;
	for (auto mmesh : area->polys)
	{
		rcPolyMesh & mesh = *mmesh;
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
			Ogre::ManualObject *m_pRecastMOWalk = mSceneMgr->createManualObject("RecastMOWalk"+to_string(count));
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
						//m_pRecastMOWalk->triangle(nIndex, nIndex + 1, nIndex + 2);
						m_pRecastMOWalk->index(nIndex + 2);
						m_pRecastMOWalk->index(nIndex + 1);
						m_pRecastMOWalk->index(nIndex);
						nIndex += 3;
					}
				}
			m_pRecastMOWalk->end();

			Ogre::SceneNode * m_pRecastSN = mSceneMgr->getRootSceneNode()->createChildSceneNode();
			m_pRecastSN->attachObject(m_pRecastMOWalk);
			//m_pRecastSN->setPosition(Vector3ToOgreVector(area->GetBoundingBox().up));
			//m_pRecastSN->setPosition(m_pRecastSN->getPosition().x, m_pRecastSN->getPosition().y, m_pRecastSN->getPosition().z);


		}
		count++;
	}
}
