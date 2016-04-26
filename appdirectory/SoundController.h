#ifndef __SOUND_CONTROLLER_H__
#define __SOUND_CONTROLLER_H__

#include "HUDListener.h"
#include "SoundBank.h"

class SoundController : public HUDListener {
public:
    SoundController(SoundBank* bank) : mBank(bank) {}

    void onHUDGuardSelect(Player* user);
    void onHUDCycleCharacter();
    void onHUDOptionSelect();
    void onHUDNavigation();

private:
    SoundBank* mBank;
};

#endif // __SOUND_CONTROLLER_H__
