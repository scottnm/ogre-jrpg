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
    mActiveState->setEnabled(false);
    mIdleState->setEnabled(true);
    mIdleState->setLoop(true);
    mIdleState->setTimePosition(0);
    mActiveState = mIdleState;
}

void AnimationController::runItemAnimation(AnimationCallback cb) {
    mActiveState->setEnabled(false);
    this->cb = cb;
    mItemState->setEnabled(true);
    mItemState->setLoop(false);
    mItemState->setTimePosition(0);
    mActiveState = mItemState;
}

void AnimationController::updateAnimationTime(Ogre::Real secondsElapsed) {
    mActiveState->addTime(secondsElapsed);
    if (mActiveState->hasEnded()) {
        cb();
    }
}
