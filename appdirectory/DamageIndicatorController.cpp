#include <DamageIndicatorController.h>

DamageIndicatorController::DamageIndicatorController(Ogre::SceneNode* cRoot)
    : characterRoot(cRoot) {}

void DamageIndicatorController::alertDamage(int dmg) {
    (void)dmg;
}

void DamageIndicatorController::alertMiss(void) {
    (void)characterRoot;
}

void DamageIndicatorController::injectTime(Ogre::Real secondsElapsed) {
    (void)secondsElapsed;
}
