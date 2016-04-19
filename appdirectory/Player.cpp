#include "Player.h"

static int idGenerator = 0;

Player::Player(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode,
        const PlayerInfo& i)
    : GameObject(_scnmgr), id(idGenerator++), info(i) {

	sceneManager = _scnmgr;
	entity = _scnmgr->createEntity("sphere.mesh");
	entity->setMaterialName("Examples/Rocky");
	entity->setCastShadows(true);

    auto targetBillboardSet = _scnmgr->createBillboardSet();
	targetBillboardSet->setMaterialName("Examples/Rockwall");
    targetBillboardSet->createBillboard(Ogre::Vector3::ZERO);

	sceneNode = _scnnode->createChildSceneNode();
	sceneNode->attachObject(entity);
	sceneNode->scale(0.5f, 0.5f, 0.5f);
    targetSceneNode = sceneNode->createChildSceneNode(Ogre::Vector3(0, 200, 0));
    targetSceneNode->attachObject(targetBillboardSet);
}
