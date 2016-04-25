#include "Player.h"

#include <OgreStringConverter.h>

static int idGenerator = 0;

Player::Player(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode,
        const PlayerInfo& i, const Ogre::Vector3& pos)
    : GameObject(_scnmgr), mInfo(i) {

    const int id = idGenerator++;

    // create the base representable object

	mEntity = _scnmgr->createEntity("ninja.mesh");
	mEntity->setCastShadows(true);
	sceneNode = _scnnode->createChildSceneNode("Player" + Ogre::StringConverter::toString(id));
	sceneNode->attachObject(mEntity);
    sceneNode->setPosition(pos);
    sceneNode->lookAt(Ogre::Vector3::ZERO, Ogre::Node::TS_WORLD);
    sceneNode->lookAt(Ogre::Vector3(0, pos.y, pos.z), Ogre::Node::TS_WORLD);
}

void Player::physicalAttack(Player& target) {
    int& targetHealth = target.mInfo.health;
    targetHealth -= this->mInfo.damage;
    if (targetHealth < 0) {
        targetHealth = 0;
    }
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
