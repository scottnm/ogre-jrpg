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
           const PlayerInfo& i, const Ogre::Vector3& pos);
	virtual ~Player(void) {};

    void physicalAttack(Player& target);
    bool isDead(void);
    void reset(void);
    const PlayerInfo& info(void);

    void setEmitting(ParticleType pt, bool emitting);
    void lookAt(GameObject* targetObject);

private:
	Ogre::Entity* mEntity;
    PlayerInfo mInfo;

    std::unordered_map<ParticleType,
                       Ogre::ParticleSystem*,
                       ParticleTypeHash> mParticleSystemMap;

    std::unordered_map<ParticleType,
                       Ogre::SceneNode*,
                       ParticleTypeHash> mParticleNodeMap;
};

#endif	// __PLAYER_H__
