#ifndef __ANIMATION_CONTROLLER_H__
#define __ANIMATION_CONTROLLER_H__

#include "AnimationCallback.h"
#include "AnimationSpec.h"
#include <OgreAnimationState.h>
#include <OgreEntity.h>

class AnimationController {
public:
    AnimationController(Ogre::Entity* mesh, const AnimationSpec& spec);
    void runIdleAnimation(void);
    void runItemAnimation(AnimationCallback cb);
    void updateAnimationTime(Ogre::Real secondsElapsed);

private:
    Ogre::AnimationState* mIdleState;
    Ogre::AnimationState* mPhysicalState;
    Ogre::AnimationState* mSpecialState;
    Ogre::AnimationState* mItemState;
    Ogre::AnimationState* mGuardState;
    Ogre::AnimationState* mDeathState;

    Ogre::AnimationState* mActiveState;

    AnimationCallback cb;
};

#endif // __ANIMATION_CONTROLLER_H__
