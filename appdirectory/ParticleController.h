#ifndef __PARTICLE_CONTROLLER_H__
#define __PARTICLE_CONTROLLER_H__

#include "ParticleCallback.h"
#include "ParticleType.h"
#include <OgreParticleSystem.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <utility>

struct ParticleEvent {
    Ogre::ParticleSystem* system;
    ParticleEndCheckCallback endCheck;
    ParticleCallback onEnd;
};

class ParticleController {
public:
    ParticleController(Ogre::SceneManager* scnMgr, Ogre::SceneNode* parent);
    void runParticleSystem(ParticleType pt, ParticleEndCheckCallback endCheck, ParticleCallback onEnd);
    void updateParticles(void);
    void stopEmittingAll(void);

    bool checkFireCollision(Ogre::SceneNode* target);
    bool checkIceCollision(Ogre::SceneNode* target);
    bool checkFlareCollision(Ogre::SceneNode* target);

private:
    bool _checkCollision(Ogre::ParticleSystem* psys, Ogre::SceneNode* target);

    static int particleControllerId;

    Ogre::ParticleSystem* guardSys;
    Ogre::ParticleSystem* itemSys;
    Ogre::ParticleSystem* physicalSys;
    Ogre::ParticleSystem* fireSys;
    Ogre::ParticleSystem* iceSys;
    Ogre::ParticleSystem* flareSys; 

    Ogre::ParticleSystem* currentSys;

    ParticleEndCheckCallback endCheck;
    ParticleCallback onEnd;
};

#endif //__PARTICLE_CONTROLLER_H__
