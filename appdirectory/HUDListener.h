#ifndef __HUD_LISTENER_H__
#define __HUD_LISTENER_H__

#include "Player.h"

class HUDListener {
public:
    virtual void onHUDPhysicalSelect(Player* attacker, Player* target) = 0;
    virtual void onHUDSpecialSelect(Player* attacker, Player* target) = 0;
    virtual void onHUDItemSelect(Player* user, Player* target) = 0;
    virtual void onHUDGuardSelect(Player* user) = 0;
    virtual void onHUDCycleCharacter() = 0;
    virtual void onHUDOptionSelect() = 0;
    virtual void onHUDNavigation() = 0;
    virtual void onHUDPlayNext() = 0;
    virtual void onHUDQuit() = 0;
};

#endif // __HUD_LISTENER_H__
