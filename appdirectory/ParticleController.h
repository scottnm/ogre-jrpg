#ifndef __PARTICLE_CONTROLLER_H__
#define __PARTICLE_CONTROLLER_H__

#include "ParticleCallback.h"
#include "ParticleType.h"
#include "SoundBank.h"
#include <OgreParticleSystem.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <string>
#include <utility>
#include <vector>

struct ParticleEvent {
    ParticleEvent(Ogre::ParticleSystem* ps, ParticleEndCheckCallback peccb,
            ParticleCallback pcb) : system(ps), endCheck(peccb), onEnd(pcb) {}

    Ogre::ParticleSystem* system;
    ParticleEndCheckCallback endCheck;
    ParticleCallback onEnd;
};

class ParticleController {
public:
    ParticleController(Ogre::SceneManager* scnMgr, Ogre::SceneNode* parent, SoundBank* soundBank);
    void runParticleSystem(ParticleType pt, ParticleEndCheckCallback endCheck,
            ParticleCallback onEnd);
    void updateParticles(void);
    void stopEmittingAll(void);

    bool checkFireCollision(Ogre::SceneNode* target);
    bool checkIceCollision(Ogre::SceneNode* target);
    bool checkFlareCollision(Ogre::SceneNode* target);

    void enableGuard(void);
    void disableGuard(void);
private:
    bool _checkCollision(Ogre::ParticleSystem* psys, Ogre::SceneNode* target, const std::string special_fx);

    static int particleControllerId;

    Ogre::ParticleSystem* systems[6];
    std::vector<ParticleEvent> mEventQueue;

    ParticleEndCheckCallback endCheck;
    ParticleCallback onEnd;

    SoundBank* mSoundBank;
};

#endif //__PARTICLE_CONTROLLER_H__
