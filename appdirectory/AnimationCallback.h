#ifndef _WIN32 // TODO: get to compile

#ifndef __ANIMATION_CALLBACK_H__
#define __ANIMATION_CALLBACK_H__

#include <functional>
using AnimationCallback = std::function<void (void)>;

#endif // __ANIMATION_CALLBACK_H__

#endif // _WIN32
