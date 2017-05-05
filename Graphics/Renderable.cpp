#include "stdafx.h"
#include "Renderable.h"
unsigned long Renderable::counter = 0;
void Renderable::_move(Renderable & other)
{
	id = other.id;
	other.id = 0;
	scene = other.scene;
	other.scene = nullptr;
	material = other.material;
	color = other.color;
}
Renderable::Renderable()
{
	to_kill = false;
	id = counter;
	counter++;
}

Renderable::Renderable(string & material): Renderable()
{
	this->material = material;
}

Renderable::Renderable(Renderable && other)
{
	_move(other);
}


Renderable & Renderable::operator=(Renderable && other)
{
	_move(other);
	return *this;
}




Renderable::~Renderable()
{
	DestroySceneAndChildren();


	if (scene)
	{

		/*if (manual_object)
		scene->getCreator()->destroyManualObject(manual_object);*/
		//((Ogre::SceneNode*) scene->getParent())->detachAllObjects();
		//scene->getCreator()->getRootSceneNode()->removeAndDestroyChild(to_string(id));

		/*Ogre::SceneNode::ObjectIterator itObject = scene->getAttachedObjectIterator();

		while (itObject.hasMoreElements())
		{
		Ogre::MovableObject* pObject = static_cast<Ogre::MovableObject*>(itObject.getNext());
		scene->getCreator()->destroyMovableObject(pObject);
		}*/

	}
}

void Renderable::DestroySceneAndChildren()
{
	scene->removeAndDestroyAllChildren();
	scene->getCreator()->destroySceneNode(scene);
}

LineStripRenderable::LineStripRenderable(LineStripRenderable && other)
{
	Renderable::_move(other);
	this->vpoints = move(other.vpoints);
	this->points = other.points;
	this->size = other.size;
}

LineStripRenderable::LineStripRenderable(Vector3 * points, unsigned size)
{
	this->points = points;
	this->size = size;
}

LineStripRenderable::LineStripRenderable(vector<Vector3>& points):Renderable()
{
	vpoints = points;
}

void LineStripRenderable::NewCreateScene(Ogre::SceneNode * parent)
{
	vector<Vector3> & points = vpoints;
	Ogre::SceneNode * scene = parent->createChildSceneNode();
	Ogre::ManualObject* m_pRecastMOPath = scene->getCreator()->createManualObject();
	m_pRecastMOPath->begin(material, Ogre::v1::RenderOperation::OT_LINE_STRIP);
	int count = 0;
	for (int i = 0; i < vpoints.size() - 1; i += 2)
	{

		m_pRecastMOPath->position(points[i].x, points[i].y + 5, -points[i].z);
		m_pRecastMOPath->colour(0, 0, 0, 255);
		m_pRecastMOPath->position(points[i + 1].x, points[i + 1].y + 5, -points[i + 1].z);
		m_pRecastMOPath->colour(0, 0, 0, 255);
		m_pRecastMOPath->index(i);
		m_pRecastMOPath->index(i + 1);
	}
	m_pRecastMOPath->end();
	scene->attachObject(m_pRecastMOPath);
}

NavMeshRenderable::NavMeshRenderable(NavMeshRenderable && other)
{
	Renderable::_move(other);
	meshes = other.meshes;
	other.meshes = nullptr;
	mesh = other.mesh;
	other.mesh = nullptr;
}

NavMeshRenderable::NavMeshRenderable(vector<rcPolyMesh*> &  meshes)
{
	this->meshes = &meshes;
}

NavMeshRenderable::NavMeshRenderable(dtNavMesh * mesh, Vector3 offset)
{
	this -> mesh = mesh;
	this->offset = offset;
	material = "NavMesh";
}

void NavMeshRenderable::NewCreateScene(Ogre::SceneNode * parent)
{
	scene = parent->createChildSceneNode();
	DrawNavMesh(*mesh, nullptr,0, material);
}



void NavMeshRenderable::drawMeshTile(const dtNavMesh& mesh, const dtNavMeshQuery* query,
	const dtMeshTile* tile, unsigned char flags, string & material)
{
	Ogre::ManualObject *man = scene->getCreator()->createManualObject();
	unsigned index_count = 0;
	dtPolyRef base = mesh.getPolyRefBase(tile);

	int tileNum = mesh.decodePolyIdTile(base);

	//dd->depthMask(false);

	man->begin(material,Ogre::v1::RenderOperation::OT_TRIANGLE_LIST);
	for (int i = 0; i < tile->header->polyCount; ++i)
	{
		const dtPoly* p = &tile->polys[i];
		if (p->getType() == DT_POLYTYPE_OFFMESH_CONNECTION)	// Skip off-mesh links.
			continue;

		const dtPolyDetail* pd = &tile->detailMeshes[i];

		//unsigned int col;
		Ogre::ColourValue col;
		if (query && query->isInClosedList(base | (dtPolyRef)i))
			col = Ogre::ColourValue(255, 196, 0, 64);
		else
		{
			if (flags & DU_DRAWNAVMESH_COLOR_TILES)
			{
				col = Ogre::ColourValue(tileNum,0,0, 128);
			}
			else
			{
				if (p->getArea() == 0) // Treat zero area type as default.
					col = Ogre::ColourValue(0, 192, 255, 64);
				else
					col = Ogre::ColourValue((p->getArea(), 64));
			}
		}

		for (int j = 0; j < pd->triCount; ++j)
		{
			const unsigned char* t = &tile->detailTris[(pd->triBase + j) * 4];
			Vector3 pos;
			for (int k = 0; k < 3; ++k)
			{
				if (t[k] < p->vertCount)
				{
					pos = *reinterpret_cast<Vector3*>(&tile->verts[p->verts[t[k]] * 3]);
					
					//dd->vertex(&tile->verts[p->verts[t[k]] * 3], col);
				}

					
				else
				{
					pos = *reinterpret_cast<Vector3*>(&tile->detailVerts[(pd->vertBase + t[k] - p->vertCount) * 3]);


					//dd->vertex(&tile->detailVerts[(pd->vertBase + t[k] - p->vertCount) * 3], col);
				}
				pos = pos + offset;
				man->position(pos.x, pos.y+2, -pos.z);
				//cout << pos->x<<" "<<pos->y<<" "<<pos->z << endl;
				man->colour(col);
			}
			man->index(index_count + 2);
			man->index(index_count + 1);
			man->index(index_count);
			index_count += 3;
		}
	}
	man->end();
	scene->attachObject(man);
	

	/*
	// Draw inter poly boundaries
	//drawPolyBoundaries(dd, tile, duRGBA(0, 48, 64, 32), 1.5f, true);

	// Draw outer poly boundaries
	//drawPolyBoundaries(dd, tile, duRGBA(0, 48, 64, 220), 2.5f, false);


	if (flags & DU_DRAWNAVMESH_OFFMESHCONS)
	{
		man->begin(DU_DRAW_LINES, 2.0f);
		for (int i = 0; i < tile->header->polyCount; ++i)
		{
			const dtPoly* p = &tile->polys[i];
			if (p->getType() != DT_POLYTYPE_OFFMESH_CONNECTION)	// Skip regular polys.
				continue;

			unsigned int col, col2;
			if (query && query->isInClosedList(base | (dtPolyRef)i))
				col = duRGBA(255, 196, 0, 220);
			else
				col = duDarkenCol(duIntToCol(p->getArea(), 220));

			const dtOffMeshConnection* con = &tile->offMeshCons[i - tile->header->offMeshBase];
			const float* va = &tile->verts[p->verts[0] * 3];
			const float* vb = &tile->verts[p->verts[1] * 3];

			// Check to see if start and end end-points have links.
			bool startSet = false;
			bool endSet = false;
			for (unsigned int k = p->firstLink; k != DT_NULL_LINK; k = tile->links[k].next)
			{
				if (tile->links[k].edge == 0)
					startSet = true;
				if (tile->links[k].edge == 1)
					endSet = true;
			}

			// End points and their on-mesh locations.
			dd->vertex(va[0], va[1], va[2], col);
			dd->vertex(con->pos[0], con->pos[1], con->pos[2], col);
			col2 = startSet ? col : duRGBA(220, 32, 16, 196);
			duAppendCircle(dd, con->pos[0], con->pos[1] + 0.1f, con->pos[2], con->rad, col2);

			dd->vertex(vb[0], vb[1], vb[2], col);
			dd->vertex(con->pos[3], con->pos[4], con->pos[5], col);
			col2 = endSet ? col : duRGBA(220, 32, 16, 196);
			duAppendCircle(dd, con->pos[3], con->pos[4] + 0.1f, con->pos[5], con->rad, col2);

			// End point vertices.
			dd->vertex(con->pos[0], con->pos[1], con->pos[2], duRGBA(0, 48, 64, 196));
			dd->vertex(con->pos[0], con->pos[1] + 0.2f, con->pos[2], duRGBA(0, 48, 64, 196));

			dd->vertex(con->pos[3], con->pos[4], con->pos[5], duRGBA(0, 48, 64, 196));
			dd->vertex(con->pos[3], con->pos[4] + 0.2f, con->pos[5], duRGBA(0, 48, 64, 196));

			// Connection arc.
			duAppendArc(dd, con->pos[0], con->pos[1], con->pos[2], con->pos[3], con->pos[4], con->pos[5], 0.25f,
				(con->flags & 1) ? 0.6f : 0, 0.6f, col);
		}
		dd->end();
	}

	const unsigned int vcol = duRGBA(0, 0, 0, 196);
	dd->begin(DU_DRAW_POINTS, 3.0f);
	for (int i = 0; i < tile->header->vertCount; ++i)
	{
		const float* v = &tile->verts[i * 3];
		dd->vertex(v[0], v[1], v[2], vcol);
	}
	dd->end();

	dd->depthMask(true);
	*/
}

void NavMeshRenderable::DrawNavMesh(const dtNavMesh& mesh, const dtNavMeshQuery* query, unsigned char flags, string & material)
{
	int indices = 0;
	for (int i = 0; i < mesh.getMaxTiles(); ++i)
	{
		const dtMeshTile* tile = mesh.getTile(i);
		if (!tile->header) continue;
		drawMeshTile(mesh, 0, tile, flags,material);
	}
}

WowModelRenderable::WowModelRenderable(Model * model, string & material):Renderable(material)
{
	this->model = model;
}

WowModelRenderable::WowModelRenderable(WowModelRenderable && other)
{
	Renderable::_move(other);
	model = other.model;
	other.model = nullptr;

}

void WowModelRenderable::NewCreateScene(Ogre::SceneNode * parent)
{
	if (model->GetVertexCount() == 0)
	{
		return;
	}
	scene = parent->createChildSceneNode();
	////Ogre::ManualObject * manual = scene->getCreator()->createManualObject();
	manual_object = scene->getCreator()->createManualObject();
	manual_object->begin(material, Ogre::v1::RenderOperation::OT_TRIANGLE_LIST);
	Vector3 * vertices = reinterpret_cast<Vector3*>(model->GetVertices());
	for (unsigned vi = 0; vi < model->GetVertexCount(); vi++)
	{
		manual_object->position(vertices[vi].x, vertices[vi].y, -vertices[vi].z);
		//Ogre::Vector3 v = Ogre::Vector3(vi % 10, vi % 12, vi % 15);
		Ogre::Vector3 v = Ogre::Vector3(vi % 10, vi % 12, vi % 15);
		manual_object->normal(v.normalisedCopy());
		if (vi % 9 == 0)
			manual_object->colour(1, 1, 1, 1);
		else if (vi % 6 == 0)
			manual_object->colour(0, 0, 0, 0);
		else
			manual_object->colour(color);

	}

	for (unsigned ii = 0; ii < model->GetIndexCount(); ii += 3)
	{
		manual_object->index(model->GetIndices()[ii + 2]);
		manual_object->index(model->GetIndices()[ii + 1]);
		manual_object->index(model->GetIndices()[ii]);
	}


	manual_object->end();

	scene->attachObject(manual_object);

}
