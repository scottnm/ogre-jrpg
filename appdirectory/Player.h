#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameObject.h"
#include "PlayerInfo.h"
#include "AnimationController.h"

#include <OgreBillboardSet.h>
#include <OgreBillboard.h>
#include <random>

// #include <ctime>

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
    GameObject* target;
    void guard(void);
    void unguard(void);
    bool isDead(void);
    void reset(void);

    const PlayerInfo& info(void) const;
    PlayerInfo& info(void);

    void setEmitting(ParticleType pt, bool emitting);
    void setVisible(ParticleType pt, bool visible);
    void lookAt(GameObject* targetObject);
    void stopEmittingAll(void);
    void checkTime(void);
    void collision(void);

    double itemStartTime;
    double physicalStartTime;
    time_t timer;

    Ogre::Real getHeight(void);

private:
	Ogre::Entity* mEntity;
    PlayerInfo mInfo;

public:
    AnimationController* mAnimationController;

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
