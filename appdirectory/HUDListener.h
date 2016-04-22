#ifndef __HUD_LISTENER_H__
#define __HUD_LISTENER_H__

class HUDListener {
public:
    virtual void onHUDPhysicalSelect() {};
    virtual void onHUDSpecialSelect() {};
    virtual void onHUDItemSelect() {};
    virtual void onHUDGuardSelect() {};
    virtual void onHUDCycleCharacter() {};
    virtual void onHUDOptionSelect() {};
    virtual void onHUDNavigation() {};
};

#endif // __HUD_LISTENER_H__
