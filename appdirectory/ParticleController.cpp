#include "ParticleController.h"

int ParticleController::particleControllerId = 0;

ParticleController::ParticleController(Ogre::SceneManager* scnMgr, Ogre::SceneNode* parent) {
    const Ogre::String id = Ogre::StringConverter::toString(particleControllerId++);

    auto guardParticles = scnMgr->createParticleSystem("Guard_P" + id, "Guard"); 
    auto guardNode = parent->createChildSceneNode("Guard_N" + id);
    guardNode->attachObject(guardParticles);
    guardGen = ParticleGenerator(guardParticles, guardNode);

    auto physicalParticles = scnMgr->createParticleSystem("Physical_P" + id, "Physical3"); 
    auto physicalNode = parent->createChildSceneNode("Physical_N" + id);
    physicalNode->attachObject(physicalParticles);
    physicalNode->setPosition(0, 125, -15);
    physicalGen = ParticleGenerator(physicalParticles, physicalNode);

    auto itemParticles = scnMgr->createParticleSystem("Item_P" + id, "Item"); 
    auto itemNode = parent->createChildSceneNode("Item_N" + id);
    itemNode->attachObject(itemParticles);
    itemNode->setPosition(0, 200, 0);
    itemGen = ParticleGenerator(itemParticles, itemNode);

    auto fireParticles = scnMgr->createParticleSystem("Fire_P" + id, "Fire"); 
    auto fireNode = parent->createChildSceneNode("Fire_N" + id);
    fireNode->attachObject(fireParticles);
    fireNode->setPosition(0, 100, 0);
    fireGen = ParticleGenerator(fireParticles, fireNode);

    auto iceParticles = scnMgr->createParticleSystem("Ice_P" + id, "Ice"); 
    auto iceNode = parent->createChildSceneNode("Ice_N" + id);
    iceNode->attachObject(iceParticles);
    iceNode->setPosition(0, 100, 0);
    iceGen = ParticleGenerator(iceParticles, iceNode);

    auto flareParticles = scnMgr->createParticleSystem("Flare_P" + id, "Flare");
    auto flareNode = parent->createChildSceneNode("Flare_N" + id);
    flareNode->attachObject(flareParticles);
    flareNode->setPosition(0, 100, 0);
    flareGen = ParticleGenerator(flareParticles, flareNode);
}

void ParticleControlle::runParticleSystem(ParticleType pt, ParticleCallback pcb) {
}

