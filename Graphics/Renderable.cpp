#include "stdafx.h"
unsigned long Renderable::counter=0;
void Renderable::_move(Renderable & other)
{
	model = other.model;
	id = other.id;
	other.id = 0;
	scene = other.scene;
	other.scene = nullptr;
}
Renderable::Renderable(Model * model):model(model)
{
	to_kill=false;
	id=counter;
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
		scene->detachAllObjects();
	scene->getCreator()->getRootSceneNode()->removeChild(scene);
}
void Renderable::CreateScene(Ogre::SceneNode * parent)
{
	if (model->GetVertexCount() == 0)
	{
		return;
	}
	string name=to_string(GetID());
	scene=parent->createChildSceneNode();
	//Ogre::SceneNode * scene =parent_scene->createChildSceneNode();
	Ogre::ManualObject * manual = scene->getCreator()->createManualObject();
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	for (int vi = 0; vi < model->GetVertexCount() * 3; vi += 3)
	{
		manual->position(model->GetVertices()[vi], model->GetVertices()[vi + 2], model->GetVertices()[vi + 1]);
		if (vi % 9 == 0)
			manual->colour(1, 1, 1, 1);
		else if (vi % 6 == 0)
			manual->colour(0, 0, 0, 0);
		else
			manual->colour(1, 1, 0, 1);
	}
	for (int ii = 0; ii < model->GetIndexCount(); ii += 3)
	{
		manual->index(model->GetIndices()[ii + 2]);
		manual->index(model->GetIndices()[ii + 1]);
		manual->index(model->GetIndices()[ii] );
	}
	manual->end();
	parent->attachObject(manual);





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
