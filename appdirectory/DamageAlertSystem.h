#ifndef __DAMAGE_ALERT_SYSTEM_H__
#define __DAMAGE_ALERT_SYSTEM_H__

#include "Player.h"
#include "MovableText.h"
#include <OgreSceneNode.h>

#include <vector>

struct DamageAlert {
    DamageAlert(Ogre::SceneNode* n, Ogre::MovableText* t)
        : node(n), text(t), secondsElapsed(0.f) {}

    Ogre::SceneNode* node;
    Ogre::MovableText* text;
    float secondsElapsed;
};

class DamageAlertSystem {
public:
    DamageAlertSystem(void);
    ~DamageAlertSystem(void);
    void addAlert(Player* playerNode, int dmgDealt);
    void updateAlerts(Ogre::Real secondsElapsed);

private:
    static constexpr float DISPLAY_TIME = 2.0f;
    std::vector<DamageAlert> mAlerts;
};

#endif // __DAMAGE_ALERT_SYSTEM_H__
