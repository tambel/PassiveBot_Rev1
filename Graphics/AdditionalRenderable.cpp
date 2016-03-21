#include "stdafx.h"
#include "AdditionalRenderable.h"


void AdditionalRenderable::_move(AdditionalRenderable & other)
{
	Renderable::_move(other);
	position = other.position;
	old_position = other.old_position;
	scene = other.scene;
	other.scene = nullptr;
	to_update = other.to_update;
}

AdditionalRenderable::AdditionalRenderable()
{

}


AdditionalRenderable::AdditionalRenderable(Model * model):
	Renderable(model)
{

}

AdditionalRenderable::AdditionalRenderable(AdditionalRenderable && other)
{
	_move(other);
}

AdditionalRenderable & AdditionalRenderable::operator=(AdditionalRenderable && other)
{
	_move(other);
	return *this;

}

AdditionalRenderable::~AdditionalRenderable()
{
}

void AdditionalRenderable::CreateScene(Ogre::SceneNode * parent)
{
	scene= parent->getCreator()->getRootSceneNode()->createChildSceneNode();
	scene->setPosition(Vector3ToOgreVector(position));

	Renderable::CreateScene(scene);
}

void AdditionalRenderable::SetPosition(Vector3 & position, bool ingame_coordinates)
{
	old_position = this->position;
	if (ingame_coordinates)
		this->position = Vector3(Metrics::MapMidPoint - position.y, -(Metrics::MapMidPoint - position.x), position.z);
	else
		this->position = position;
	if (old_position != position && !to_update)
	{
		to_update = true;
		old_position = this->position;
	}


}

void AdditionalRenderable::UpdatePosition()
{
	if (to_update)
	{
		scene->setPosition(Vector3ToOgreVector(position));
		to_update = false;
	}
	
}