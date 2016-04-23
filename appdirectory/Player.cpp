#include "Player.h"

Player::Player(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode,
        const PlayerInfo& i)
    : GameObject(_scnmgr), mInfo(i) {

	sceneManager = _scnmgr;
	mEntity = _scnmgr->createEntity("sphere.mesh");
	mEntity->setMaterialName("Examples/Rocky");
	mEntity->setCastShadows(true);

	sceneNode = _scnnode->createChildSceneNode();
	sceneNode->attachObject(mEntity);
	sceneNode->scale(0.5f, 0.5f, 0.5f);
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
