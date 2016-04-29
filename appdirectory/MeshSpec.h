#ifndef __MESHSPEC_H__
#define __MESHSPEC_H__

#include "AnimationSpec.h"
#include <string>

struct MeshSpec {
    MeshSpec(const std::string& n, const std::string& idle, const std::string& phys,
            const std::string& special, const std::string& item,
            const std::string& guard, const std::string& death)
        : name(n), animationSpec(idle, phys, special, item, guard, death) {}

    const std::string name;
    const AnimationSpec animationSpec;
};
#endif // __MESHSPEC_H__
