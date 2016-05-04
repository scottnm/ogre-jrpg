#include <DamageIndicatorController.h>
#include <OgreMath.h>

Ogre::Vector3 DamageIndicatorController::endPos(0, 50, 0);

DamageIndicatorController::DamageIndicatorController(Ogre::SceneNode* cRoot)
    : mCharacterRoot(cRoot) {}

void DamageIndicatorController::alertDamage(int dmg) {
    (void)dmg;
}

void DamageIndicatorController::alertMiss(void) {
    (void)mCharacterRoot;
}

void DamageIndicatorController::injectTime(Ogre::Real secondsElapsed) {
    auto i = mIndicators.begin();
    while (i != mIndicators.end()) {
        DamageIndicator& di = *i;
        if (/* time over */ di.second <= secondsElapsed) {
            // hide the scene node
            di.first->setVisible(false);
            // remove it from the indicator list
            i = mIndicators.erase(i);
        }
        else {
            // update the time of the indicator
            di.second -= secondsElapsed;
            // translate the indicator
            di.first->translate(
                    Ogre::Math::lerp(di.first->getPosition(), endPos, secondsElapsed));
            ++i; 
        }
    }
}
