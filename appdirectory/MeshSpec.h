#pragma once

#include "AnimationSpec.h"
#include <string>

struct MeshSpec {
    MeshSpec(const std::string& n, const AnimationSpec& as)
        : name(n), animationSpec(as) {}

    const std::string name;
    const AnimationSpec animationSpec;
};
