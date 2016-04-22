#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameObject.h"
#include "PlayerInfo.h"

#include <OgreBillboardSet.h>
#include <OgreBillboard.h>

class Player : public GameObject
{
public:
	Player(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode,
           const PlayerInfo& i);
	virtual ~Player(void) {};

    void physicalAttack(Player& target);

	Ogre::Entity* entity;
    PlayerInfo info;
};

#endif	// __PLAYER_H__
