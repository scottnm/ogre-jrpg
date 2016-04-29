#include "AnimationController.h"

AnimationController::AnimationController(Ogre::Entity* mesh, const AnimationSpec& spec) {
    mIdleState = mesh->getAnimationState(spec.idleState);
    mPhysicalState = mesh->getAnimationState(spec.physicalAttackState);
    mSpecialState = mesh->getAnimationState(spec.specialAttackState);
    mItemState = mesh->getAnimationState(spec.itemState);
    mGuardState = mesh->getAnimationState(spec.guardState);
    mDeathState = mesh->getAnimationState(spec.deathState);

    mActiveState = mIdleState;
}

void AnimationController::runIdleAnimation(void) {
    mIdleState->setEnabled(true);
    mIdleState->setLoop(true);
    mActiveState = mIdleState;
}

void AnimationController::updateAnimationTime(Ogre::Real secondsElapsed) {
    mActiveState->addTime(secondsElapsed);
}
