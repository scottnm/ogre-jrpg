#include "GameObject.h"

void GameObject::setPosition(Ogre::Vector3 position) {
	sceneNode->setPosition(position);
}

void GameObject::lookAt(GameObject* targetObject) {
	Ogre::SceneNode* targetNode = targetObject->sceneNode;
	sceneNode->lookAt(sceneNode->convertWorldToLocalPosition(targetNode->_getDerivedPosition()),
        Ogre::Node::TransformSpace::TS_LOCAL, Ogre::Vector3::NEGATIVE_UNIT_Z);
	for (size_t i = 0; i < particleNodeList.size(); ++i)
	{
		Ogre::SceneNode* psNode = particleNodeList[i];
		psNode->lookAt(psNode->convertWorldToLocalPosition(targetNode->_getDerivedPosition()),
	        Ogre::Node::TransformSpace::TS_LOCAL, Ogre::Vector3::NEGATIVE_UNIT_Z);
	}
}
