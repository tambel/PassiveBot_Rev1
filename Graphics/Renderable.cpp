#include "stdafx.h"
unsigned long Renderable::counter=0;
Renderable::Renderable(Model<int> * model):model(model)
{
	to_kill=false;
	id=counter;
	counter++;
}


Renderable::~Renderable(void)
{
	scene->detachAllObjects();
}
bool Renderable::CreateScene(Ogre::SceneNode * parent)
{
	if (model->GetVertexCount() == 0)
	{
		return false;
	}
	string name=to_string(GetID());
	scene=parent->createChildSceneNode();
	//Ogre::SceneNode * scene =parent_scene->createChildSceneNode();
	Ogre::ManualObject * manual = scene->getCreator()->createManualObject();
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	for (unsigned i=0;i<model->GetVertexCount();i++)
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
	return true;
}
