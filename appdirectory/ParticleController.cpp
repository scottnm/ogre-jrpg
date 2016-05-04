#include "ParticleController.h"
#include <OgreParticle.h>

int ParticleController::particleControllerId = 0;

ParticleController::ParticleController(Ogre::SceneManager* scnMgr, Ogre::SceneNode* parent) {
    const Ogre::String id = Ogre::StringConverter::toString(particleControllerId++);

    auto guardParticles = scnMgr->createParticleSystem("Guard_P" + id, "Guard"); 
    guardParticles->setEmitting(false);
    auto guardNode = parent->createChildSceneNode("Guard_N" + id);
    guardNode->attachObject(guardParticles);
    systems[PT_Guard] = guardParticles;

    auto physicalParticles = scnMgr->createParticleSystem("Physical_P" + id, "Physical3"); 
    physicalParticles->setEmitting(false);
    auto physicalNode = parent->createChildSceneNode("Physical_N" + id);
    physicalNode->attachObject(physicalParticles);
    physicalNode->setPosition(0, 125, -15);
    systems[PT_Physical] = physicalParticles;

    auto itemParticles = scnMgr->createParticleSystem("Item_P" + id, "Item"); 
    itemParticles->setEmitting(false);
    auto itemNode = parent->createChildSceneNode("Item_N" + id);
    itemNode->attachObject(itemParticles);
    itemNode->setPosition(0, 200, 0);
    systems[PT_Item] = itemParticles;

    auto fireParticles = scnMgr->createParticleSystem("Fire_P" + id, "Fire"); 
    fireParticles->setEmitting(false);
    auto fireNode = parent->createChildSceneNode("Fire_N" + id);
    fireNode->attachObject(fireParticles);
    fireNode->setPosition(0, 100, 0);
    systems[PT_Fire] = fireParticles;

    auto iceParticles = scnMgr->createParticleSystem("Ice_P" + id, "Ice"); 
    iceParticles->setEmitting(false);
    auto iceNode = parent->createChildSceneNode("Ice_N" + id);
    iceNode->attachObject(iceParticles);
    iceNode->setPosition(0, 100, 0);
    systems[PT_Ice] = iceParticles;

    auto flareParticles = scnMgr->createParticleSystem("Flare_P" + id, "Flare");
    flareParticles->setEmitting(false);
    auto flareNode = parent->createChildSceneNode("Flare_N" + id);
    flareNode->attachObject(flareParticles);
    flareNode->setPosition(0, 100, 0);
    systems[PT_Flare] = flareParticles;
}

void ParticleController::stopEmittingAll(void) {
    for (int i = 0; i < 6; ++i) {
        auto ps = systems[i];
        ps->setEmitting(false);
        ps->setVisible(false);
    }
}

void ParticleController::runParticleSystem(ParticleType pt, ParticleEndCheckCallback endCheck,
        ParticleCallback onEnd) {  
    Ogre::ParticleSystem* system = systems[pt];
    system->setEmitting(true);
    system->setVisible(true);
    mEventQueue.push_back(ParticleEvent(system, endCheck, onEnd));
}

void ParticleController::updateParticles(void) {
    auto i = mEventQueue.begin();
    while (i != mEventQueue.end()) {
        ParticleEvent& evt = *i;
        if (evt.endCheck()) {
            evt.onEnd();
            evt.system->setEmitting(false);
            evt.system->setVisible(false);
            i = mEventQueue.erase(i);
        }
        else {
            ++i;
        }
    }
}

void ParticleController::enableGuard(void) {
    auto guardSys = systems[PT_Guard];
    guardSys->setEmitting(true);
    guardSys->setVisible(true);
}

void ParticleController::disableGuard(void) {
    auto guardSys = systems[PT_Guard];
    guardSys->setEmitting(false);
    guardSys->setVisible(false);
}

bool ParticleController::checkFireCollision(Ogre::SceneNode* target) {
    return _checkCollision(systems[PT_Fire], target);
}

bool ParticleController::checkIceCollision(Ogre::SceneNode* target) {
    return _checkCollision(systems[PT_Ice], target);
}

bool ParticleController::checkFlareCollision(Ogre::SceneNode* target) {
    return _checkCollision(systems[PT_Flare], target);
}

bool ParticleController::_checkCollision(Ogre::ParticleSystem* psys, Ogre::SceneNode* target) {
    int numParticles = psys->getNumParticles();
    for(int n = 0; n < numParticles; ++n) {
        auto scnNode = psys->getParentSceneNode();
        bool hit = target->_getWorldAABB().intersects(
                scnNode->_getDerivedPosition() +
                (scnNode->_getDerivedOrientation() * psys->getParticle(n)->position));
        if(hit) {
            return true;
        }
    }
    return false;
}
