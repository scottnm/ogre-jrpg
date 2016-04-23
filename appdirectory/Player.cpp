#include "Player.h"

Player::Player(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode,
        const PlayerInfo& i)
    : GameObject(_scnmgr), info(i) {

	sceneManager = _scnmgr;
	entity = _scnmgr->createEntity("sphere.mesh");
	entity->setMaterialName("Examples/Rocky");
	entity->setCastShadows(true);

	sceneNode = _scnnode->createChildSceneNode();
	sceneNode->attachObject(entity);
	sceneNode->scale(0.5f, 0.5f, 0.5f);
}

void Player::physicalAttack(Player& target) {
    int& targetHealth = target.info.health;
    targetHealth -= this->info.damage;
    if (targetHealth < 0) {
        targetHealth = 0;
    }
}

bool Player::isDead(void) {
    return info.health == 0;
}
