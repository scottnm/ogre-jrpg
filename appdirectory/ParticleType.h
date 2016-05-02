#ifndef __PARTICLE_TYPE_H__
#define __PARTICLE_TYPE_H__

#include <stddef.h> // for size_t

enum class ParticleType {
    Guard = 0,
    Item,
    Physical,
    Fire,
    Ice,
    Flare,
    NumParticleTypes
};

typedef struct ParticleTypeHash {
    template <typename T>
    size_t operator()(T t) const {
        return static_cast<size_t>(t);
    }
} ParticleTypeHash;

#endif //__PARTICLE_TYPE_H__
