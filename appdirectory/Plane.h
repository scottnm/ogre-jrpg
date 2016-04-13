#ifndef _PLANE_H_
#define _PLANE_H_

#include <Ogre.h>

class Plane
{
public:
	Plane(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode);
	~Plane(void);

	Ogre::SceneNode* sceneNode;
	Ogre::Entity* entity;
};

#endif // _PLANE_H_