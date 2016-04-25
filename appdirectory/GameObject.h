#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "ParticleType.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreParticleSystem.h>

#include <unordered_map>

class GameObject
{
public:
    GameObject(Ogre::SceneManager* _scnmgr) : sceneManager(_scnmgr) {}
    virtual ~GameObject(void) {}

    void setPosition(Ogre::Vector3 position);
    virtual void lookAt(GameObject* targetObject);

    Ogre::SceneManager* sceneManager;
    Ogre::SceneNode* sceneNode;

};

#endif // __GAME_OBJECT_H__
