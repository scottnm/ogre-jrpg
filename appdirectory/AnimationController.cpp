#include "AnimationController.h"

AnimationController::AnimationController(Ogre::Entity* mesh, const AnimationSpec& spec)
    : idleAnim(mesh->getAnimationState(spec.idle.first), spec.idle.second),
      physicalAnim(mesh->getAnimationState(spec.physical.first), spec.physical.second),
      specialAnim(mesh->getAnimationState(spec.special.first), spec.special.second),
      itemAnim(mesh->getAnimationState(spec.item.first), spec.item.second),
      guardAnim(mesh->getAnimationState(spec.guard.first), spec.guard.second),
      deathAnim(mesh->getAnimationState(spec.idle.first), spec.idle.second),
      activeAnim(&idleAnim) {
}

void AnimationController::runIdleAnimation(void) {
    activeAnim->first->setEnabled(false);
    activeAnim = &idleAnim;
    idleAnim.first->setEnabled(true);
    idleAnim.first->setLoop(true);
    idleAnim.first->setTimePosition(0);
}

void AnimationController::runAnimation(AnimationType at, AnimationCallback cb) {
    activeAnim->first->setEnabled(false);
    this->cb = cb;
    switch (at) {
        case AnimationType::Physical:
            activeAnim = &physicalAnim;
            break;
        case AnimationType::Special:
            activeAnim = &specialAnim;
            break;
        case AnimationType::Item:
            activeAnim = &itemAnim;
            break;
        case AnimationType::Guard:
            activeAnim = &guardAnim;
            break;
        case AnimationType::Death:
            activeAnim = &guardAnim;
            break;
    }
    activeAnim->first->setEnabled(true);
    activeAnim->first->setLoop(false);
    activeAnim->first->setTimePosition(0);
}

void AnimationController::updateAnimationTime(Ogre::Real secondsElapsed) {
    activeAnim->first->addTime(secondsElapsed * activeAnim->second);
    if (activeAnim->first->hasEnded()) {
        cb();
    }
}
