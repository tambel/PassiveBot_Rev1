#include "stdafx.h"
#include "Renderable.h"
unsigned long Renderable::counter = 0;
void Renderable::_move(Renderable & other)
{
	model = other.model;
	id = other.id;
	other.id = 0;
	scene = other.scene;
	other.scene = nullptr;
}
Renderable::Renderable()
{

}
Renderable::Renderable(Model * model) :model(model)
{
	to_kill = false;
	id = counter;
	counter++;
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
	if (scene)
	{

		/*if (manual_object)
		scene->getCreator()->destroyManualObject(manual_object);*/
		((Ogre::SceneNode*) scene->getParent())->detachAllObjects();
		//scene->getCreator()->getRootSceneNode()->removeAndDestroyChild(to_string(id));

		/*Ogre::SceneNode::ObjectIterator itObject = scene->getAttachedObjectIterator();

		while (itObject.hasMoreElements())
		{
		Ogre::MovableObject* pObject = static_cast<Ogre::MovableObject*>(itObject.getNext());
		scene->getCreator()->destroyMovableObject(pObject);
		}*/

	}
}
void Renderable::CreateScene(Ogre::SceneNode * parent, string & material, Ogre::ColourValue & color)
{
	if (model->GetVertexCount() == 0)
	{
		return;
	}
	
	string name = to_string(GetID());
	scene = parent->createChildSceneNode();
	////Ogre::ManualObject * manual = scene->getCreator()->createManualObject();
	manual_object = scene->getCreator()->createManualObject();
	manual_object->begin(material, Ogre::v1::RenderOperation::OT_TRIANGLE_LIST);
	//manual_object->position(-100, 0, 0);
	//manual_object->normal(0, 1, 0);
	//manual_object->tangent(1.0f, 0.0f, 0.0f);
	////manual_object->textureCoord(0, 0);
	//manual_object->colour(255, 1, 0, 1);

	//manual_object->position(0, 0, -100);
	//manual_object->normal(0, 1, 0);
	//manual_object->tangent(1.0f, 0.0f, 0.0f);
	////manual_object->textureCoord(0, 1);
	//manual_object->colour(255, 1, 0, 1);

	//manual_object->position(-100, 0, -100);
	//manual_object->normal(0, 1, 0);
	//manual_object->tangent(1.0f, 0.0f, 0.0f);
	////manual_object->textureCoord(1, 1);
	//manual_object->colour(255, 1, 0, 1);

	//manual_object->index(0);
	//
	//manual_object->index(1);
	//manual_object->index(2);

	//manual_object->index(1);
	//manual_object->index(0);
	//manual_object->index(2);

	
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
	parent->attachObject(manual_object);





	/*for (unsigned i=0;i<model->GetVertexCount()*3;i+=3)
	{
	manual->position(Vector3ToOgreVector(model->GetVertices()[i].position));
	manual->colour(ColorToOgreColor(model->GetVertices()[i].color));
	}
	for (unsigned long i=0;i<model->GetIndexCount();i++)
	{
	manual->index(model->GetIndices()[i]);
	}
	manual->end();
	Ogre::MeshPtr mesh= manual->convertToMesh(name+"_mesh");

	Ogre::Entity* entity = scene->getCreator()->createEntity(name+"_entity", mesh->getName());
	scene->attachObject(entity);
	scene->getCreator()->destroyManualObject(manual);
	return true;*/
}

LineStripRenderable::LineStripRenderable(LineStripRenderable && other)
{
	this->points = other.points;
	this->size = other.size;
}

LineStripRenderable::LineStripRenderable(Vector3 * points, unsigned size)
{
	this->points = points;
	this->size = size;
}

void LineStripRenderable::CreateScene(Ogre::SceneNode * parent)
{
	//string name = to_string(GetID());
	Ogre::SceneNode * scene = parent->createChildSceneNode();
	Ogre::ManualObject* m_pRecastMOPath = scene->getCreator()->createManualObject();
	m_pRecastMOPath->begin("mat", Ogre::v1::RenderOperation::OT_LINE_STRIP);
	for (int i = 0; i < size; i++)
	{
		m_pRecastMOPath->position(points[i].x, points[i].z, points[i].y + 2);
		//m_pRecastMOPath->position(points[i].x, points[i].y, points[i].z + 2);
		m_pRecastMOPath->colour(1, 0, 0);
	}
	m_pRecastMOPath->end();
	scene->attachObject(m_pRecastMOPath);
}

NavMeshRenderable::NavMeshRenderable(NavMeshRenderable && other)
{
	meshes = other.meshes;
	other.meshes = nullptr;
}

NavMeshRenderable::NavMeshRenderable(vector<rcPolyMesh*> &  meshes)
{
	this->meshes = &meshes;
}

void NavMeshRenderable::CreateScene(Ogre::SceneNode * parent, string & material, Ogre::ColourValue & color)
{
	int count = 0;
	scene = parent->createChildSceneNode();
	for (auto mmesh : *meshes)
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
			Ogre::ManualObject *manual_object = scene->getCreator()->createManualObject();
			manual_object->begin(material, Ogre::v1::RenderOperation::OT_TRIANGLE_LIST);
			for (int i = 0; i < mesh.npolys; ++i) // go through all polygons
												  //if (mesh.areas[i] == 0)
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

						manual_object->position(x, y, -z);
						Ogre::Vector3 vec = Ogre::Vector3(i % 10, i % 12, i % 15);
						manual_object->normal(vec.normalisedCopy());
						if (mesh.areas[i] == 0)
							manual_object->colour(1, 0.5, 0, 1);
						else
							manual_object->colour(1, 1, 1, 0);

					}
					manual_object->index(nIndex + 2);
					manual_object->index(nIndex + 1);
					manual_object->index(nIndex);
					nIndex += 3;
				}
			}
			manual_object->end();


			scene->attachObject(manual_object);

		}
	}
}
