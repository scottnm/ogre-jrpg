#include "ParticleController.h"
#include <OgreParticle.h>

int ParticleController::particleControllerId = 0;

ParticleController::ParticleController(Ogre::SceneManager* scnMgr, Ogre::SceneNode* parent)
    : currentSys(nullptr) {
    const Ogre::String id = Ogre::StringConverter::toString(particleControllerId++);

    auto guardParticles = scnMgr->createParticleSystem("Guard_P" + id, "Guard"); 
    guardParticles->setEmitting(false);
    auto guardNode = parent->createChildSceneNode("Guard_N" + id);
    guardNode->attachObject(guardParticles);
    guardSys = guardParticles;

    auto physicalParticles = scnMgr->createParticleSystem("Physical_P" + id, "Physical3"); 
    physicalParticles->setEmitting(false);
    auto physicalNode = parent->createChildSceneNode("Physical_N" + id);
    physicalNode->attachObject(physicalParticles);
    physicalNode->setPosition(0, 125, -15);
    physicalSys = physicalParticles;

    auto itemParticles = scnMgr->createParticleSystem("Item_P" + id, "Item"); 
    itemParticles->setEmitting(false);
    auto itemNode = parent->createChildSceneNode("Item_N" + id);
    itemNode->attachObject(itemParticles);
    itemNode->setPosition(0, 200, 0);
    itemSys = itemParticles;

    auto fireParticles = scnMgr->createParticleSystem("Fire_P" + id, "Fire"); 
    fireParticles->setEmitting(false);
    auto fireNode = parent->createChildSceneNode("Fire_N" + id);
    fireNode->attachObject(fireParticles);
    fireNode->setPosition(0, 100, 0);
    fireSys = fireParticles;

    auto iceParticles = scnMgr->createParticleSystem("Ice_P" + id, "Ice"); 
    iceParticles->setEmitting(false);
    auto iceNode = parent->createChildSceneNode("Ice_N" + id);
    iceNode->attachObject(iceParticles);
    iceNode->setPosition(0, 100, 0);
    iceSys = iceParticles;

    auto flareParticles = scnMgr->createParticleSystem("Flare_P" + id, "Flare");
    flareParticles->setEmitting(false);
    auto flareNode = parent->createChildSceneNode("Flare_N" + id);
    flareNode->attachObject(flareParticles);
    flareNode->setPosition(0, 100, 0);
    flareSys = flareParticles;
}

void ParticleController::stopEmittingAll(void) {
    guardSys->setEmitting(false);
    guardSys->setVisible(false);
    physicalSys->setEmitting(false);
    physicalSys->setVisible(false);
    itemSys->setEmitting(false);
    itemSys->setVisible(false);
    fireSys->setEmitting(false);
    fireSys->setVisible(false);
    iceSys->setEmitting(false);
    iceSys->setVisible(false);
    flareSys->setEmitting(false);
    flareSys->setVisible(false);
}

void ParticleController::runParticleSystem(ParticleType pt, ParticleEndCheckCallback endCheck,
        ParticleCallback onEnd) {  
    if (currentSys != nullptr) {
        this->onEnd(); 
        currentSys->setEmitting(false);
        currentSys->setVisible(false);
        currentSys = nullptr;
    }

    this->endCheck = endCheck;
    this->onEnd = onEnd;
    switch(pt) {
        case ParticleType::Guard:
            currentSys = guardSys;
            break;
        case ParticleType::Item:
            currentSys = itemSys;
            break;
        case ParticleType::Physical:
            currentSys = physicalSys;
            break;
        case ParticleType::Fire:
            currentSys = fireSys;
            break;
        case ParticleType::Ice:
            currentSys = iceSys;
            break;
        case ParticleType::Flare:
            currentSys = flareSys;
            break;
        default:
            break;
    }
    currentSys->setEmitting(true);
    currentSys->setVisible(true);
}

void ParticleController::updateParticles(void) {
    if (currentSys == nullptr) {
        return;
    }

    if (endCheck()) {
        onEnd();
        currentSys->setEmitting(false);
        currentSys->setVisible(false);
        currentSys = nullptr;
    }
}


bool ParticleController::checkFireCollision(Ogre::SceneNode* target) {
    return _checkCollision(fireSys, target);
}

bool ParticleController::checkIceCollision(Ogre::SceneNode* target) {
    return _checkCollision(iceSys, target);
}

bool ParticleController::checkFlareCollision(Ogre::SceneNode* target) {
    return _checkCollision(flareSys, target);
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
