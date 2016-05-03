#include "ParticleController.h"
#include <OgreParticle.h>

int ParticleController::particleControllerId = 0;

ParticleController::ParticleController(Ogre::SceneManager* scnMgr, Ogre::SceneNode* parent)
    : currentGen(nullptr) {
    const Ogre::String id = Ogre::StringConverter::toString(particleControllerId++);

    auto guardParticles = scnMgr->createParticleSystem("Guard_P" + id, "Guard"); 
    guardParticles->setEmitting(false);
    auto guardNode = parent->createChildSceneNode("Guard_N" + id);
    guardNode->attachObject(guardParticles);
    guardGen = ParticleGenerator(guardParticles, guardNode);

    auto physicalParticles = scnMgr->createParticleSystem("Physical_P" + id, "Physical3"); 
    physicalParticles->setEmitting(false);
    auto physicalNode = parent->createChildSceneNode("Physical_N" + id);
    physicalNode->attachObject(physicalParticles);
    physicalNode->setPosition(0, 125, -15);
    physicalGen = ParticleGenerator(physicalParticles, physicalNode);

    auto itemParticles = scnMgr->createParticleSystem("Item_P" + id, "Item"); 
    itemParticles->setEmitting(false);
    auto itemNode = parent->createChildSceneNode("Item_N" + id);
    itemNode->attachObject(itemParticles);
    itemNode->setPosition(0, 200, 0);
    itemGen = ParticleGenerator(itemParticles, itemNode);

    auto fireParticles = scnMgr->createParticleSystem("Fire_P" + id, "Fire"); 
    fireParticles->setEmitting(false);
    auto fireNode = parent->createChildSceneNode("Fire_N" + id);
    fireNode->attachObject(fireParticles);
    fireNode->setPosition(0, 100, 0);
    fireGen = ParticleGenerator(fireParticles, fireNode);

    auto iceParticles = scnMgr->createParticleSystem("Ice_P" + id, "Ice"); 
    iceParticles->setEmitting(false);
    auto iceNode = parent->createChildSceneNode("Ice_N" + id);
    iceNode->attachObject(iceParticles);
    iceNode->setPosition(0, 100, 0);
    iceGen = ParticleGenerator(iceParticles, iceNode);

    auto flareParticles = scnMgr->createParticleSystem("Flare_P" + id, "Flare");
    flareParticles->setEmitting(false);
    auto flareNode = parent->createChildSceneNode("Flare_N" + id);
    flareNode->attachObject(flareParticles);
    flareNode->setPosition(0, 100, 0);
    flareGen = ParticleGenerator(flareParticles, flareNode);
}

void ParticleController::stopEmittingAll(void) {
    guardGen.first->setEmitting(false);
    physicalGen.first->setEmitting(false);
    itemGen.first->setEmitting(false);
    fireGen.first->setEmitting(false);
    iceGen.first->setEmitting(false);
    flareGen.first->setEmitting(false);
}

void ParticleController::runParticleSystem(ParticleType pt, ParticleEndCheckCallback endCheck,
        ParticleCallback onEnd) {  
    this->endCheck = endCheck;
    this->onEnd = onEnd;
    switch(pt) {
        case ParticleType::Guard:
            currentGen = &guardGen;
            break;
        case ParticleType::Item:
            currentGen = &itemGen;
            break;
        case ParticleType::Physical:
            currentGen = &physicalGen;
            break;
        case ParticleType::Fire:
            currentGen = &fireGen;
            break;
        case ParticleType::Ice:
            currentGen = &iceGen;
            break;
        case ParticleType::Flare:
            currentGen = &flareGen;
            break;
        default:
            break;
    }
    currentGen->first->setEmitting(true);
    currentGen->first->setVisible(true);
}

void ParticleController::updateParticles(void) {
    if (currentGen == nullptr) {
        std::cout << "empty gen" << std::endl;
        return;
    }

    std::cout << "not empty gen" << std::endl;
    if (endCheck()) {
        onEnd();
        currentGen->first->setEmitting(false);
        currentGen->first->setVisible(false);
        currentGen = nullptr;
    }
    else {
        std::cout << "failed colliion" << std::endl;
    }
}


bool ParticleController::checkFireCollision(Ogre::SceneNode* target) {
    return _checkCollision(fireGen.first, target);
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
