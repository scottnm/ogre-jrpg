#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameObject.h"
#include "HUDTargetable.h"
#include "PlayerInfo.h"

#include <OgreBillboardSet.h>
#include <OgreBillboard.h>

class Player : public GameObject,
               public HUDTargetable
{
public:
	Player(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode,
           const PlayerInfo& i);
	virtual ~Player(void) {};

	void hideTargetArrow();
	void showTargetArrow();

    const int id;
	Ogre::Entity* entity;
    PlayerInfo info;
    Ogre::SceneNode* targetSceneNode;
};

#endif	// __PLAYER_H__
