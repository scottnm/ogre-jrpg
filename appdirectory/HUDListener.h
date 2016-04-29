#ifndef __HUD_LISTENER_H__
#define __HUD_LISTENER_H__

#include "Player.h"

class HUDListener {
public:
    virtual void onHUDPhysicalSelect(Player* attacker, Player* target) {};
    virtual void onHUDSpecialSelect(Player* attacker, Player* target) {};
    virtual void onHUDItemSelect(Player* user, Player* target) {};
    virtual void onHUDGuardSelect(Player* user) {};
    virtual void onHUDCycleCharacter() {};
    virtual void onHUDOptionSelect() {};
    virtual void onHUDNavigation() {};
    virtual void onHUDPlayAgain() {};
    virtual void onHUDQuit() {};
};

#endif // __HUD_LISTENER_H__
