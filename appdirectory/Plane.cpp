#include "Plane.h"

Plane::Plane(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode) {
	std::cout << "Create Plane" << std::endl;
	sceneNode = _scnnode->createChildSceneNode();
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane(
		"plane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 2000, 2000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z
	);
	entity = _scnmgr->createEntity("plane");
	entity->setCastShadows(false);
	entity->setMaterialName("Examples/Rockwall");
	sceneNode->attachObject(entity);
}