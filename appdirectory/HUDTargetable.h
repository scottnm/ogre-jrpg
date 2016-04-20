#ifndef __HUD_TARGETABLE_H__
#define __HUD_TARGETABLE_H__

class HUDTargetable {
public:
    virtual void showTargetArrow() = 0;
    virtual void hideTargetArrow() = 0;
};

#endif // __HUD_TARGETABLE_H__
