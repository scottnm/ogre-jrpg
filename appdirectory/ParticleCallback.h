#ifndef __PARTICLE_CALLBACK_H__
#define __PARTICLE_CALLBACK_H__

#include <functional>
using ParticleEndCheckCallback = std::function<bool (void)>;
using ParticleCallback = std::function<void (void)>;

#endif // __PARTICLE_CALLBACK_H__
