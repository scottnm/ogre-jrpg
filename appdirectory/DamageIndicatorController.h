#ifndef __DAMAGE_INDICATOR_CONTROLLER_H__
#define __DAMAGE_INDICATOR_CONTROLLER_H__

#include <vector>
#include <OgreSceneNode.h>
#include <OgreFrameListener.h>

class DamageIndicatorController {
public:
    DamageIndicatorController(Ogre::SceneNode* cRoot);
    void alertDamage(int dmg);
    void alertMiss(void);
    void injectTime(Ogre::Real secondsElapsed);
private:
    Ogre::SceneNode* characterRoot;
    std::vector<Ogre::SceneNode*> damageIndicatorNodes; 
};

#endif // __DAMAGE_INDICATOR_CONTROLLER_H__
