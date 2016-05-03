#ifndef __PARTICLE_CONTROLLER_H__
#define __PARTICLE_CONTROLLER_H__

#include "ParticleCallback.h"
#include "ParticleType.h"
#include <OgreParticleSystem.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <utility>

typedef std::pair<Ogre::ParticleSystem*, Ogre::SceneNode*> ParticleGenerator;

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

    ParticleGenerator guardGen;
    ParticleGenerator itemGen;
    ParticleGenerator physicalGen;
    ParticleGenerator fireGen;
    ParticleGenerator iceGen;
    ParticleGenerator flareGen; 

    ParticleGenerator* currentGen;

    ParticleEndCheckCallback endCheck;
    ParticleCallback onEnd;
};

#endif //__PARTICLE_CONTROLLER_H__
