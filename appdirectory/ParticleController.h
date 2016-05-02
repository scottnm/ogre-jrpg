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
    ParticleController(Ogre::SceneManager* scnMgr, Ogre::SceneNode* parent);
    void runParticleSystem(ParticleType pt, ParticleEndCheckCallback endCheck, ParticleCallback onEnd);
    void updateParticles(void);
    void stopEmittingAll(void);

private:
    static int particleControllerId;
    static ParticleGenerator emptyGen;

    ParticleGenerator guardGen;
    ParticleGenerator itemGen;
    ParticleGenerator physicalGen;
    ParticleGenerator fireGen;
    ParticleGenerator iceGen;
    ParticleGenerator flareGen; 

    ParticleGenerator& currentGen;

    ParticleEndCheckCallback endCheck;
    ParticleCallback onEnd;
};

#endif //__PARTICLE_CONTROLLER_H__
