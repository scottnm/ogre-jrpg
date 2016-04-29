#ifndef __ANIMATION_CONTROLLER_H__
#define __ANIMATION_CONTROLLER_H__

#include "AnimationSpec.h"
#include <OgreAnimationState.h>
#include <OgreEntity.h>

class AnimationController {
public:
    AnimationController(Ogre::Entity* mesh, const AnimationSpec& spec);
    void runIdleAnimation(void);
    void updateAnimationTime(Ogre::Real secondsElapsed);

private:
    Ogre::AnimationState* mIdleState;
    Ogre::AnimationState* mPhysicalState;
    Ogre::AnimationState* mSpecialState;
    Ogre::AnimationState* mItemState;
    Ogre::AnimationState* mGuardState;
    Ogre::AnimationState* mDeathState;

    Ogre::AnimationState* mActiveState;
};

#endif // __ANIMATION_CONTROLLER_H__
