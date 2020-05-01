#pragma once

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreFrameListener.h>
#include <utility>
#include <vector>

typedef std::pair<Ogre::SceneNode*, Ogre::Real> DamageIndicator;

class DamageIndicatorController {
public:
    DamageIndicatorController(Ogre::SceneManager* scnMgr, Ogre::SceneNode* cRoot);
    void alertDamage(int dmg);
    void alertMiss(void);
    void injectTime(Ogre::Real secondsElapsed);
private:
    Ogre::SceneManager* mSceneManager;
    Ogre::SceneNode* mCharacterRoot;
    std::vector<DamageIndicator> mIndicators;

    static Ogre::Vector3 endPos;
};
