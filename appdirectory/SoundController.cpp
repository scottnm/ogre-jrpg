#include "SoundController.h"

void SoundController::onHUDGuardSelect() {
    mBank->play("guard_fx");
}

void SoundController::onHUDCycleCharacter() {
    mBank->play("hud_cycle_fx");
}

void SoundController::onHUDOptionSelect() {
    mBank->play("option_select_fx");
}

void SoundController::onHUDNavigation() {
    mBank->play("option_nav_fx");
}
