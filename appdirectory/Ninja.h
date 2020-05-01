#pragma once

#include "GameObject.h"
#include <string>

class Ninja : public GameObject
{
public:

	enum particle_type {
		guard,
		item,
		physical,
		fire,
		ice,
		flare
	};

	Ninja(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode, int i);
	virtual ~Ninja(void) {};

    void startGuardSystem(bool start);
    void startItemSystem(bool start);
    void startPhysicalSystem(bool start);
    void startFireSystem(bool start);
    void startIceSystem(bool start);
    void startFlareSystem(bool start);

    void visibilityGuardSystem(bool start);
    void visibilityItemSystem(bool start);
    void visibilityPhysicalSystem(bool start);
    void visibilityFireSystem(bool start);
    void visibilityIceSystem(bool start);
    void visibilityFlareSystem(bool start);

    bool is_emitting(particle_type p_type);

	Ogre::Entity* entity;
};