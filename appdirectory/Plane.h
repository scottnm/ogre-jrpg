#pragma once

#include <Ogre.h>

class Plane
{
public:
	Plane(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode);
	~Plane(void);

	Ogre::SceneNode* sceneNode;
	Ogre::Entity* entity;
};