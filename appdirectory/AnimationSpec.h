#ifndef __ANIMATION_SPEC_H__
#define __ANIMATION_SPEC_H__

#include <string>

struct AnimationSpec {
    AnimationSpec(const std::string& idle, const std::string& phys,
            const std::string& special, const std::string& item,
            const std::string& guard, const std::string& death)
        : idleState(idle),
          physicalAttackState(phys),
          specialAttackState(special),
          itemState(item),
          guardState(guard),
          deathState(death) {}

    const std::string idleState;
    const std::string physicalAttackState;
    const std::string specialAttackState;
    const std::string itemState;
    const std::string guardState;
    const std::string deathState;
};

#endif // __ANIMATION_SPEC_H__
