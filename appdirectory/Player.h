#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GameObject.h"

class Player : public GameObject
{
public:
	Player(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode);
	virtual ~Player(void) {};

	Ogre::Entity* entity;
};

#endif	// ifndef _PLAYER_H_