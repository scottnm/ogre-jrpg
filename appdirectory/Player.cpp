#include "Player.h"

#include <OgreStringConverter.h>
#include <OgreParticle.h>
// #include <cstdlib>

// static int seed = 0;

static int idGenerator = 0;

Player::Player(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode,
        const PlayerInfo& i, const Ogre::Vector3& pos)
    : GameObject(_scnmgr), mInfo(i) {

    const Ogre::String id = Ogre::StringConverter::toString(idGenerator++);

    // create the base representable object

	mEntity = _scnmgr->createEntity("ninja.mesh");
	mEntity->setCastShadows(true);
	sceneNode = _scnnode->createChildSceneNode("Player" + id);
	sceneNode->attachObject(mEntity);
    sceneNode->setPosition(pos);
    sceneNode->lookAt(Ogre::Vector3::ZERO, Ogre::Node::TS_WORLD);
    sceneNode->lookAt(Ogre::Vector3(0, pos.y, pos.z), Ogre::Node::TS_WORLD);

    auto guardParticles = _scnmgr->createParticleSystem("Guard_P" + id, "Guard"); 
    auto guardNode = sceneNode->createChildSceneNode("Guard_N" + id);
    guardNode->attachObject(guardParticles);
    mParticleSystemMap.emplace(ParticleType::Guard, guardParticles);
    mParticleNodeMap.emplace(ParticleType::Guard, guardNode);
    guardParticles->setEmitting(false);

    
    auto physicalParticles = _scnmgr->createParticleSystem("Physical_P" + id, "Physical2"); 
    auto physicalNode = sceneNode->createChildSceneNode("Physical_N" + id);
    physicalNode->attachObject(physicalParticles);
    physicalNode->setPosition(25, 90, -80);
    mParticleSystemMap.emplace(ParticleType::Physical, physicalParticles);
    mParticleNodeMap.emplace(ParticleType::Physical, physicalNode);
    physicalParticles->setEmitting(false);

    auto itemParticles = _scnmgr->createParticleSystem("Item_P" + id, "Item"); 
    auto itemNode = sceneNode->createChildSceneNode("Item_N" + id);
    itemNode->attachObject(itemParticles);
    // itemNode->setPosition(0, 100, -50);
    mParticleSystemMap.emplace(ParticleType::Item, itemParticles);
    mParticleNodeMap.emplace(ParticleType::Item, itemNode);
    itemParticles->setEmitting(false);

    auto fireParticles = _scnmgr->createParticleSystem("Fire_P" + id, "Fire"); 
    auto fireNode = sceneNode->createChildSceneNode("Fire_N" + id);
    fireNode->attachObject(fireParticles);
    fireNode->setPosition(0, 100, 0);
    mParticleSystemMap.emplace(ParticleType::Fire, fireParticles);
    mParticleNodeMap.emplace(ParticleType::Fire, fireNode);
    fireParticles->setEmitting(false);

    auto iceParticles = _scnmgr->createParticleSystem("Ice_P" + id, "Ice"); 
    auto iceNode = sceneNode->createChildSceneNode("Ice_N" + id);
    iceNode->attachObject(iceParticles);
    iceNode->setPosition(0, 100, 0);
    mParticleSystemMap.emplace(ParticleType::Ice, iceParticles);
    mParticleNodeMap.emplace(ParticleType::Ice, iceNode);
    iceParticles->setEmitting(false);

    auto flareParticles = _scnmgr->createParticleSystem("Flare_P" + id, "Flare"); 
    auto flareNode = sceneNode->createChildSceneNode("Flare_N" + id);
    flareNode->attachObject(flareParticles);
    flareNode->setPosition(0, 100, 0);
    mParticleSystemMap.emplace(ParticleType::Flare, flareParticles);
    mParticleNodeMap.emplace(ParticleType::Flare, flareNode);
    flareParticles->setEmitting(false);

    target = NULL;
    
}

void Player::physicalAttack(Player& target) {
    int& targetHealth = target.mInfo.health;
    targetHealth -= this->mInfo.damage;
    if (targetHealth < 0) {
        targetHealth = 0;
    }
    setEmitting(ParticleType::Physical, true);
    setVisible(ParticleType::Physical, true);
}

void Player::specialAttack(Player& target) {
    int& targetHealth = target.mInfo.health;
    targetHealth -= this->mInfo.damage;
    if (targetHealth < 0) {
        targetHealth = 0;
    }
    int randNum = rand() % 3;
    std::cout << randNum << std::endl << std::endl << std::endl;
    ParticleType pt;
    switch(randNum) {
        case 0:
            pt = ParticleType::Fire;
            setEmitting(pt, true);
            setVisible(pt, true);
            break;
        case 1:
            pt = ParticleType::Ice;
            setEmitting(pt, true);
            setVisible(pt, true);
            break;
        case 2:
            pt = ParticleType::Flare;
            setEmitting(pt, true);
            setVisible(pt, true);
            break;
        default:
            break;
    }
}

void Player::item(Player& target) {
    auto pt = ParticleType::Item;
    // auto itemSystem = mParticleSystemMap.find(pt)->second;
    auto itemNode = mParticleNodeMap.find(pt)->second;
    auto targetNode = target.sceneNode;
    itemNode->setPosition(GameObject::sceneNode->convertWorldToLocalPosition(targetNode->_getDerivedPosition()) + Ogre::Vector3(0, 200, 0));
    setEmitting(pt, true);
    setVisible(pt, true);
}

bool Player::isDead(void) {
    return mInfo.health == 0;
}

void Player::reset(void) {
    mInfo.reset();
}

const PlayerInfo& Player::info(void) {
    return mInfo;
}

void Player::setEmitting(ParticleType pt, bool emitting) {
    mParticleSystemMap.find(pt)->second->setEmitting(emitting);
}


void Player::setVisible(ParticleType pt, bool visible) {
	mParticleSystemMap.find(pt)->second->setVisible(visible);
}

void Player::lookAt(GameObject* targetObject) {
    target = targetObject;
    GameObject::lookAt(targetObject);
    auto targetNode = targetObject->sceneNode;
    for(auto particleNodePair : mParticleNodeMap) {
        auto psNode = particleNodePair.second;
		psNode->lookAt(psNode->convertWorldToLocalPosition(targetNode->_getDerivedPosition() + Ogre::Vector3(0, 100, 0)),
	        Ogre::Node::TransformSpace::TS_LOCAL, Ogre::Vector3::NEGATIVE_UNIT_Z);
    }
}

void Player::stopEmittingAll(void) {
    setEmitting(ParticleType::Guard, false);
    setVisible(ParticleType::Guard, false);
    setEmitting(ParticleType::Physical, false);
    setVisible(ParticleType::Physical, false);
    setEmitting(ParticleType::Item, false);
    setVisible(ParticleType::Item, false);
    setEmitting(ParticleType::Fire, false);
    setVisible(ParticleType::Fire, false);
    setEmitting(ParticleType::Ice, false);
    setVisible(ParticleType::Ice, false);
    setEmitting(ParticleType::Flare, false);
    setVisible(ParticleType::Flare, false);
}

void Player::collision(void) {
    for(auto particleSystemPair : mParticleSystemMap) {
        auto ps = particleSystemPair.second;
        if(ps->getEmitting()) {
            int numParticles = ps->getNumParticles();
            for(int particleNum = 0; particleNum < numParticles; ++particleNum) {
                auto p = ps->getParticle(particleNum);
                if(p) {
                    auto pPos = p->position;
                    auto sn = ps->getParentSceneNode();
                    auto orientation = sn->_getDerivedOrientation();
                    auto snPos = sn->_getDerivedPosition();
                    auto direction = orientation * pPos;
                    auto pWorldPos = snPos + direction;
                    if(target) {
                        auto targetBoundingBox = target->sceneNode->_getWorldAABB();
                        bool hit = targetBoundingBox.intersects(pWorldPos);
                        if(hit) {
                            if(particleNum == numParticles - 1) {
                                setVisible(particleSystemPair.first, false);
                            }
                            setEmitting(particleSystemPair.first, false);
                            std::cout << "hit" << std::endl;
                        }
                    }
                }
            }
        }
    }
}
