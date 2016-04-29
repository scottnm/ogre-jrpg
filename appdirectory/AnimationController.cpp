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

void AnimationController::runAnimation(AnimationType at, AnimationCallback cb) {
    mActiveState->setEnabled(false);
    this->cb = cb;

    switch (at) {
        case AnimationType::Physical:
            mActiveState = mPhysicalState;
            break;
        case AnimationType::Special:
            mActiveState = mSpecialState;
            break;
        case AnimationType::Item:
            mActiveState = mItemState;
            break;
        case AnimationType::Guard:
            mActiveState = mGuardState;
            break;
        case AnimationType::Death:
            mActiveState = mDeathState;
            break;
    }

    mActiveState->setEnabled(true);
    mActiveState->setLoop(false);
    mActiveState->setTimePosition(0);
}

void AnimationController::updateAnimationTime(Ogre::Real secondsElapsed) {
    mActiveState->addTime(secondsElapsed);
    if (mActiveState->hasEnded()) {
        cb();
    }
}
