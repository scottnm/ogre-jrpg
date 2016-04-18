#include "Player.h"

Player::Player(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode,
        const PlayerInfo& i) : GameObject(_scnmgr), info(i) {
	GameObject::sceneManager = _scnmgr;
	entity = _scnmgr->createEntity("sphere.mesh");
	entity->setMaterialName("Examples/Rocky");
	entity->setCastShadows(true);
	GameObject::sceneNode = _scnnode->createChildSceneNode();
	GameObject::sceneNode->attachObject(entity);
	GameObject::sceneNode->scale(0.5f, 0.5f, 0.5f);
}
