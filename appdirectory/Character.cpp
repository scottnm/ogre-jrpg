#ifndef _WIN32 // TODO: get to compile

#include "Character.h"

Character::Character(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode) :
GameObject(_scnmgr) {
	head = _scnmgr->createEntity("sphere.mesh");
	head->setMaterialName("Examples/Rockwall");
	head->setCastShadows(false);
	body = _scnmgr->createEntity("cube.mesh");
	body->setMaterialName("Examples/Rocky");
	body->setCastShadows(false);
	leg = _scnmgr->createEntity("cube.mesh");
	leg->setMaterialName("Examples/Rocky");
	leg->setCastShadows(false);
	leg2 = _scnmgr->createEntity("cube.mesh");
	leg2->setMaterialName("Examples/Rocky");
	leg2->setCastShadows(false);
	arm = _scnmgr->createEntity("cube.mesh");
	arm->setMaterialName("Examples/Rocky");
	arm->setCastShadows(false);
	arm2 = _scnmgr->createEntity("cube.mesh");
	arm2->setMaterialName("Examples/Rocky");
	arm2->setCastShadows(false);
	GameObject::sceneNode = _scnnode->createChildSceneNode();
	Ogre::SceneNode* tempNode = GameObject::sceneNode;
	headNode = tempNode->createChildSceneNode();
	legNode = tempNode->createChildSceneNode();
	legNode2 = tempNode->createChildSceneNode();
	armNode = tempNode->createChildSceneNode();
	armNode2 = tempNode->createChildSceneNode();
	GameObject::sceneNode->attachObject(body);
	headNode->attachObject(head);
	legNode->attachObject(leg);
	legNode2->attachObject(leg2);
	armNode->attachObject(arm);
	armNode2->attachObject(arm2);
	headNode->setPosition(0, 100, 0);
	legNode->setPosition(30, -75, 0);
	legNode2->setPosition(-30, -75, 0);
	armNode->setPosition(75, 25, 0);
	armNode2->setPosition(-75, 25, 0);
	sceneNode->setPosition(0, 50, 0);
	headNode->scale(0.5, 0.5, 0.5);
	legNode->scale(0.5, 0.5, 0.5);
	legNode2->scale(0.5, 0.5, 0.5);
	armNode->scale(0.5, 0.25, 0.25);
	armNode2->scale(0.5, 0.25, 0.25);

	// Set up particle systems
	guard = _scnmgr->createParticleSystem("Guard", "Guard");
	guardNode = GameObject::sceneNode->createChildSceneNode();
	guardNode->attachObject(guard);
	// guard->setEmitting(false);
	item = _scnmgr->createParticleSystem("Item", "Item");
	itemNode = GameObject::sceneNode->createChildSceneNode();
	itemNode->setPosition(0, 200, 0);
	itemNode->attachObject(item);
	// item->setEmitting(false);
	hitNode = GameObject::sceneNode->createChildSceneNode();
	physicalNode = armNode->createChildSceneNode();
	fireNode = headNode->createChildSceneNode();
	iceNode = headNode->createChildSceneNode();
	flareNode = headNode->createChildSceneNode();
}

// void Character::addParticleSystem(std::string name, int num) {
// 	std::string ps_name;
// 	Ogre::ParticleSystem* ps;
// 	Ogre::SceneNode* ps_node;
// 	if(num == 0)
// 	{
// 		ps_name = name + "h";
// 		ps = GameObject::sceneManager->createParticleSystem(ps_name, name);
// 		ps_node = headNode->createChildSceneNode();
// 		ps_node->attachObject(ps);
// 		particleList.push_back(ps);
// 		particleNodeList.push_back(ps_node);
// 	}
// 	else if(num == 1)
// 	{
// 		ps_name = name + "b";
// 		ps = GameObject::sceneManager->createParticleSystem(ps_name, name);
// 		ps_node = GameObject::sceneNode->createChildSceneNode();
// 		ps_node->attachObject(ps);
// 		particleList.push_back(ps);
// 		particleNodeList.push_back(ps_node);
// 	}
// 	else if(num == 2)
// 	{
// 		ps_name = name + "l";
// 		ps = GameObject::sceneManager->createParticleSystem(ps_name, name);
// 		ps_node = legNode->createChildSceneNode();
// 		ps_node->attachObject(ps);
// 		particleList.push_back(ps);
// 		particleNodeList.push_back(ps_node);
// 	}
// 	else if(num == 3)
// 	{
// 		ps_name = name + "l2";
// 		ps = GameObject::sceneManager->createParticleSystem(ps_name, name);
// 		ps_node = legNode2->createChildSceneNode();
// 		ps_node->attachObject(ps);
// 		particleList.push_back(ps);
// 		particleNodeList.push_back(ps_node);
// 	}
// 	else if(num == 4)
// 	{
// 		ps_name = name + "a";
// 		ps = GameObject::sceneManager->createParticleSystem(ps_name, name);
// 		ps_node = armNode->createChildSceneNode();
// 		ps_node->attachObject(ps);
// 		particleList.push_back(ps);
// 		particleNodeList.push_back(ps_node);
// 	}
// 	else if(num == 5)
// 	{
// 		ps_name = name + "a2";
// 		ps = GameObject::sceneManager->createParticleSystem(ps_name, name);
// 		ps_node = armNode2->createChildSceneNode();
// 		ps_node->attachObject(ps);
// 		particleList.push_back(ps);
// 		particleNodeList.push_back(ps_node);
// 	}
// }

#endif // _WIN32