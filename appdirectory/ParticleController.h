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
    void runParticleSystem(ParticleType pt, ParticleCallback pcb);

private:
    static int particleControllerId;
    ParticleGenerator guardGen;
    ParticleGenerator itemGen;
    ParticleGenerator physicalGen;
    ParticleGenerator fireGen;
    ParticleGenerator iceGen;
    ParticleGenerator flareGen; 

    ParticleCallback cb;
};

#endif //__PARTICLE_CONTROLLER_H__
