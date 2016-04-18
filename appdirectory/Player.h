#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GameObject.h"
#include "PlayerInfo.h"

class Player : public GameObject
{
public:
	Player(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode,
           const PlayerInfo& i);
	virtual ~Player(void) {};

    const int id;
	Ogre::Entity* entity;
    PlayerInfo info;
};

#endif	// ifndef _PLAYER_H_
