#ifndef _WIN32 // TODO: get to compile

#pragma once

#include <functional>
using ParticleEndCheckCallback = std::function<bool (void)>;
using ParticleCallback = std::function<void (void)>;

#endif // _WIN32
