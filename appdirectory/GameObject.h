#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

class GameObject
{
public:
    GameObject(Ogre::SceneManager* _scnmgr) : sceneManager(_scnmgr) {}
    ~GameObject(void) {}

    Ogre::SceneManager* sceneManager;
    Ogre::SceneNode* sceneNode;
};

#endif // __GAME_OBJECT_H__
