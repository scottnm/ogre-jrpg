#ifndef	_PARTICLE_H_
#define _PARTICLE_H_

#include "GameObject.h"
#include "OgreParticleSystem.h"

class Particle : public GameObject
{
public:
	Particle(Ogre::SceneManger* _scnmgr, Ogre::SceneNode* _scnnode);
	virtual ~Particle(void) {};

	Ogre::ParticleSystem* particleSystem;
	// void switchParticleSystem(const char* )
};

#endif // _PARTICLE_H_