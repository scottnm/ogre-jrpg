#ifndef __HUD_TARGETABLE_H__
#define __HUD_TARGETABLE_H__

class HUDTargetable {
public:
    virtual void target() = 0;
    virtual void untarget() = 0;
};

#endif // __HUD_TARGETABLE_H__
