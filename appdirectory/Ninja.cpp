#ifndef _WIN32 // TODO: get to compile

#include "Ninja.h"

Ninja::Ninja(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode, int i) :
GameObject(_scnmgr) {
	entity = _scnmgr->createEntity("ninja.mesh");
	Ogre::SceneNode* ninjaNode = _scnnode->createChildSceneNode();
	ninjaNode->yaw(Ogre::Degree(90));
	ninjaNode->attachObject(entity);
	GameObject::sceneNode = ninjaNode;
	std::vector<Ogre::ParticleSystem*> psList;
	std::vector<Ogre::SceneNode*> psNodeList;
	std::string name = "guard" + std::to_string(i*6);
	Ogre::ParticleSystem* ps = _scnmgr->createParticleSystem(name, "Guard");
	Ogre::SceneNode* psNode = ninjaNode->createChildSceneNode();
	psNode->attachObject(ps);
	psList.push_back(ps);
	psNodeList.push_back(psNode);
	name = "item" + std::to_string(1 + i*6);
	ps = _scnmgr->createParticleSystem(name, "Item");
	psNode = ninjaNode->createChildSceneNode();
	psNode->attachObject(ps);
	psNode->setPosition(0, 100, -50);
	psList.push_back(ps);
	psNodeList.push_back(psNode);
	name = "physical" + std::to_string(2 + i*6);
	ps = _scnmgr->createParticleSystem(name, "Physical2");
	psNode = ninjaNode->createChildSceneNode();
	psNode->attachObject(ps);
	psNode->setPosition(25, 90, -80);
	psList.push_back(ps);
	psNodeList.push_back(psNode);
	name = "fire" + std::to_string(3 + i*6);
	ps = _scnmgr->createParticleSystem(name, "Fire");
	psNode = ninjaNode->createChildSceneNode();
	psNode->attachObject(ps);
	psList.push_back(ps);
	psNodeList.push_back(psNode);
	psNode->setPosition(0, 100, 0);
	name = "ice" + std::to_string(4 + i*6);
	ps = _scnmgr->createParticleSystem(name, "Ice");
	psNode = ninjaNode->createChildSceneNode();
	psNode->attachObject(ps);
	psList.push_back(ps);
	psNodeList.push_back(psNode);
	psNode->setPosition(0, 100, 0);
	name = "flare" + std::to_string(5 + i*6);
	ps = _scnmgr->createParticleSystem(name, "Flare");
	psNode = ninjaNode->createChildSceneNode();
	psNode->attachObject(ps);
	psList.push_back(ps);
	psNodeList.push_back(psNode);
	psNode->setPosition(0, 100, 0);
	GameObject::particleSystemList = psList;
	GameObject::particleNodeList = psNodeList;
	startGuardSystem(false);
	startItemSystem(false);
	startPhysicalSystem(false);
	startFireSystem(false);
	startIceSystem(false);
	startFlareSystem(false);
}

void Ninja::startGuardSystem(bool start) {
	Ogre::ParticleSystem* gd = GameObject::particleSystemList[guard];
	if(gd)
		gd->setEmitting(start);
}

void Ninja::startItemSystem(bool start) {
	Ogre::ParticleSystem* itm = GameObject::particleSystemList[item];
	if(itm)
		itm->setEmitting(start);
}

void Ninja::startPhysicalSystem(bool start) {
	Ogre::ParticleSystem* phys = GameObject::particleSystemList[physical];
	if(phys)
		phys->setEmitting(start);
}

void Ninja::startFireSystem(bool start) {
	Ogre::ParticleSystem* fr = GameObject::particleSystemList[fire];
	if(fr)
		fr->setEmitting(start);
}

void Ninja::startIceSystem(bool start) {
	Ogre::ParticleSystem* i = GameObject::particleSystemList[ice];
	if(i)
		i->setEmitting(start);
}

void Ninja::startFlareSystem(bool start) {
	Ogre::ParticleSystem* fl = GameObject::particleSystemList[flare];
	if(fl)
		fl->setEmitting(start);
}

void Ninja::visibilityGuardSystem(bool start) {
	Ogre::ParticleSystem* gd = GameObject::particleSystemList[guard];
	if(gd)
		gd->setVisible(start);
}

void Ninja::visibilityItemSystem(bool start) {
	Ogre::ParticleSystem* itm = GameObject::particleSystemList[item];
	if(itm)
		itm->setVisible(start);
}

void Ninja::visibilityPhysicalSystem(bool start) {
	Ogre::ParticleSystem* phys = GameObject::particleSystemList[physical];
	if(phys)
		phys->setVisible(start);
}

void Ninja::visibilityFireSystem(bool start) {
	Ogre::ParticleSystem* fr = GameObject::particleSystemList[fire];
	if(fr)
		fr->setVisible(start);
}

void Ninja::visibilityIceSystem(bool start) {
	Ogre::ParticleSystem* i = GameObject::particleSystemList[ice];
	if(i)
		i->setVisible(start);
}

void Ninja::visibilityFlareSystem(bool start) {
	Ogre::ParticleSystem* fl = GameObject::particleSystemList[flare];
	if(fl)
		fl->setVisible(start);
}


bool Ninja::is_emitting(particle_type p_type) {
	Ogre::ParticleSystem* ps = GameObject::particleSystemList[p_type];
	return ps->getEmitting();
}

#endif // _WIN32
