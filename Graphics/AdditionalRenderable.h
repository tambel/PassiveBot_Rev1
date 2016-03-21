#pragma once
#include "stdafx.h"
class AdditionalRenderable:public Renderable
{
	Ogre::SceneNode * scene;
	Vector3 position;
	Vector3 old_position;
	bool to_update = false;;
	void _move(AdditionalRenderable & other);
public:
	AdditionalRenderable();
	AdditionalRenderable(Model * model);
	AdditionalRenderable(AdditionalRenderable && other);
	AdditionalRenderable & operator=(AdditionalRenderable&& other);
	~AdditionalRenderable();

	void CreateScene(Ogre::SceneNode * parent);
	void SetPosition(Vector3 & position, bool ingame_coordinates = false);
	void UpdatePosition();
};

