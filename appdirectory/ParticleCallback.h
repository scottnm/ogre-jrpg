#pragma once

#include <functional>
using ParticleEndCheckCallback = std::function<bool (void)>;
using ParticleCallback = std::function<void (void)>;
