#ifndef	_CHARACTER_H_
#define	_CHARACTER_H_

#include "GameObject.h"
// #include <string>
// #include "Particle.h"

class Character : public GameObject
{
public:
	Character(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode);
	virtual ~Character(void) {};

	// void addParticleSystem(std::string name, int num);

	Ogre::Entity* head;
	Ogre::Entity* body;
	Ogre::Entity* leg;
	Ogre::Entity* leg2;
	Ogre::Entity* arm;
	Ogre::Entity* arm2;
	Ogre::SceneNode* headNode;
	Ogre::SceneNode* legNode;
	Ogre::SceneNode* legNode2;
	Ogre::SceneNode* armNode;
	Ogre::SceneNode* armNode2;
	Ogre::ParticleSystem* guard;
	Ogre::ParticleSystem* item;
	Ogre::ParticleSystem* physical;
	Ogre::ParticleSystem* fire;
	Ogre::ParticleSystem* ice;
	Ogre::ParticleSystem* flare;
	Ogre::ParticleSystem* hit;
	Ogre::SceneNode* guardNode;
	Ogre::SceneNode* itemNode;
	Ogre::SceneNode* physicalNode;
	Ogre::SceneNode* fireNode;
	Ogre::SceneNode* iceNode;
	Ogre::SceneNode* flareNode;
	Ogre::SceneNode* hitNode;
	// std::vector<Ogre::ParticleSystem*> particleList;
	// std::vector<Ogre::SceneNode*> particleNodeList;
};

#endif	// _CHARACTER_H_