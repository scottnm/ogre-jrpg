#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <OgreParticleSystem.h>

class GameObject
{
public:
    GameObject(Ogre::SceneManager* _scnmgr) : sceneManager(_scnmgr) {}
    virtual ~GameObject(void) {}

    void setPosition(Ogre::Vector3 position);

    Ogre::SceneManager* sceneManager;
    Ogre::SceneNode* sceneNode;
};

#endif // __GAME_OBJECT_H__
