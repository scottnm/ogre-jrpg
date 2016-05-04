#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "AnimationController.h"
#include "GameObject.h"
#include "ParticleController.h"
#include "PlayerInfo.h"
#include "SoundBank.h"

#include <OgreBillboardSet.h>
#include <OgreBillboard.h>
#include <random>

class Player : public GameObject
{
public:
	Player(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode,
           const PlayerInfo& i, const Ogre::Vector3& pos);
	virtual ~Player(void) {};

    bool attemptPhysicalAttack(void);
    void physicalAttack(Player& target);
    void specialAttack(Player& target);
    void item(void);
    Player* target;
    void guard(void);
    void unguard(void);
    bool isDead(void);
    void reset(void);

    const PlayerInfo& info(void) const;
    PlayerInfo& info(void);

    using GameObject::lookAt;
    void lookAt(Player* targetObject);

    Ogre::Real getHeight(void);

private:
	Ogre::Entity* mEntity;
    PlayerInfo mInfo;

public:
    AnimationController* mAnimationController;
    ParticleController* mParticleController;

private:

    std::unordered_map<ParticleType,
                       Ogre::ParticleSystem*,
                       ParticleTypeHash> mParticleSystemMap;

    std::unordered_map<ParticleType,
                       Ogre::SceneNode*,
                       ParticleTypeHash> mParticleNodeMap;
    static std::default_random_engine rand_generator;
    static std::uniform_real_distribution<float> rand_dist;
};

#endif	// __PLAYER_H__
