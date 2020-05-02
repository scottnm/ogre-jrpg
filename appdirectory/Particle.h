#ifndef _WIN32 // TODO: get to compile

#pragma once

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

#endif // _WIN32