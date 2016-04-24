#ifndef __DAMAGE_ALERT_SYSTEM_H__
#define __DAMAGE_ALERT_SYSTEM_H__

#include "Player.h"
#include <OgreSceneNode.h>
#include <OgreFrameListener.h>
#include <OgreCamera.h>
#include "MovableTextOverlay.h"
#include <vector>

struct DamageAlert {
    MovableTextOverlay* mto;
    float secondsElapsed;
};

class DamageAlertSystem : public Ogre::FrameListener {
public:
    DamageAlertSystem(Ogre::Camera& c);
    ~DamageAlertSystem(void);
    void addAlert(Player* playerNode, int dmgDealt);
    void updateAlerts(Ogre::Real secondsElapsed);

private:
    bool frameStarted(const Ogre::FrameEvent& evt);

    static constexpr float DISPLAY_TIME = 2.0f;
    std::vector<DamageAlert> mAlerts;
    Ogre::Camera& mCamera;
};

#endif // __DAMAGE_ALERT_SYSTEM_H__
