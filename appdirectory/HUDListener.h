#ifndef __HUD_LISTENER_H__
#define __HUD_LISTENER_H__

class HUDListener {
public:
    virtual void onHUDPhysicalSelect() = 0;
    virtual void onHUDSpecialSelect() = 0;
    virtual void onHUDItemSelect() = 0;
    virtual void onHUDGuardSelect() = 0;
    virtual void onHUDCycleCharacter() = 0;
};

#endif // __HUD_LISTENER_H__
