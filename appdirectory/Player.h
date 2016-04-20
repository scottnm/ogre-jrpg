#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GameObject.h"
// #include "HUDTargetable.h"
#include "PlayerInfo.h"

#include <OgreBillboardSet.h>
#include <OgreBillboard.h>

class Player : public GameObject
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

#endif	// ifndef _PLAYER_H_
