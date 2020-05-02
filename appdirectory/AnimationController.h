#ifndef _WIN32 // TODO: get to compile

#pragma once

#include "AnimationCallback.h"
#include "AnimationSpec.h"
#include <OgreAnimationState.h>
#include <OgreEntity.h>
#include <utility>

enum class AnimationType {
    Physical,
    Special,
    Item,
    Guard,
    Death
};

typedef std::pair<Ogre::AnimationState*, float> Animation;

class AnimationController {
public:
    AnimationController(Ogre::Entity* mesh, const AnimationSpec& spec);
    void runIdleAnimation(void);
    void runAnimation(AnimationType at, AnimationCallback cb);
    void updateAnimationTime(Ogre::Real secondsElapsed);

private:
    const Animation idleAnim;
    const Animation physicalAnim;
    const Animation specialAnim;
    const Animation itemAnim;
    const Animation guardAnim;
    const Animation deathAnim;

    const Animation* activeAnim;

    AnimationCallback cb;
};

#endif // _WIN32
