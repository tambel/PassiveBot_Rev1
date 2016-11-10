#include "stdafx.h"
#include "Renderable.h"
unsigned long Renderable::counter=0;
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
Renderable::Renderable(Model * model):model(model)
{
	to_kill = false;
	id = counter;
	counter++;
}





Renderable::Renderable(Renderable && other)
{
	_move(other);
}


Renderable & Renderable::operator=( Renderable && other)
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
void Renderable::CreateScene(Ogre::SceneNode * parent)
{
	if (model->GetVertexCount() == 0)
	{
		return;
	}
	string name=to_string(GetID());
	scene=parent->createChildSceneNode(name);
	//Ogre::ManualObject * manual = scene->getCreator()->createManualObject();
	manual_object= scene->getCreator()->createManualObject();
	manual_object->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	for (unsigned vi = 0; vi < model->GetVertexCount() * 3; vi += 3)
	{
		manual_object->position(model->GetVertices()[vi], model->GetVertices()[vi + 2], model->GetVertices()[vi + 1]);
		if (vi % 9 == 0)
			manual_object->colour(1, 1, 1, 1);
		else if (vi % 6 == 0)
			manual_object->colour(0, 0, 0, 0);
		else
			manual_object->colour(1, 1, 0, 1);
	}
	for (unsigned ii = 0; ii < model->GetIndexCount(); ii += 3)
	{
		manual_object->index(model->GetIndices()[ii + 2]);
		manual_object->index(model->GetIndices()[ii + 1]);
		manual_object->index(model->GetIndices()[ii] );
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
	m_pRecastMOPath->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
	for (int i = 0; i < size; i++)
	{
		m_pRecastMOPath->position(points[i].x, points[i].z, points[i].y + 2);
		m_pRecastMOPath->colour(1, 0, 0);
	}
	m_pRecastMOPath->end();
	scene->attachObject(m_pRecastMOPath);
}
