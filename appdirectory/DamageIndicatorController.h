#ifndef __DAMAGE_INDICATOR_CONTROLLER_H__
#define __DAMAGE_INDICATOR_CONTROLLER_H__

#include <OgreSceneNode.h>
#include <OgreFrameListener.h>
#include <utility>
#include <vector>

typedef std::pair<Ogre::SceneNode*, Ogre::Real> DamageIndicator;

class DamageIndicatorController {
public:
    DamageIndicatorController(Ogre::SceneNode* cRoot);
    void alertDamage(int dmg);
    void alertMiss(void);
    void injectTime(Ogre::Real secondsElapsed);
private:
    Ogre::SceneNode* mCharacterRoot;
    std::vector<DamageIndicator> mIndicators; 

    static Ogre::Vector3 endPos;
};

#endif // __DAMAGE_INDICATOR_CONTROLLER_H__
