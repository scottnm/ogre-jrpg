#pragma once

#include <stddef.h> // for size_t

enum ParticleType {
    PT_Guard = 0,
    PT_Item,
    PT_Physical,
    PT_Fire,
    PT_Ice,
    PT_Flare
};

typedef struct ParticleTypeHash {
    template <typename T>
    size_t operator()(T t) const {
        return static_cast<size_t>(t);
    }
} ParticleTypeHash;
