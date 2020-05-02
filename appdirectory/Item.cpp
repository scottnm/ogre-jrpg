#ifndef _WIN32 // TODO: get to compile

#include "Item.h"
#include <OgreMath.h>

Item::Item(std::string name, std::string description, bool isOffensive, int d_health, int d_armor,
	       int d_damage, int d_special, float d_accuracy)
	: name(name), description(description), isOffensive(isOffensive), delta_health(d_health), delta_armor(d_armor),
	  delta_damage(d_damage), delta_special(d_special), delta_accuracy(d_accuracy) {}

void Item::use(Player& target) {
    PlayerInfo& info = target.info();

    int health = std::max(info.health + delta_health, 0);
    info.health = std::min(health, info.healthMax);
    if (delta_health != 0) {
        target.mDamageIndicatorController->alertDamage(Ogre::Math::Abs(delta_health));
    }

    int special = std::max(info.specialPoints + delta_special, 0);
    info.specialPoints = std::min(special, info.specialPointsMax);

	info.armor = std::max(info.armor + delta_armor, 0);
	info.damage = std::max(info.damage + delta_damage, 0);

    info.accuracy = std::max(info.accuracy * (1 + delta_accuracy), 0.0f);
    info.accuracy = std::min(info.accuracy, 1.0f);
}

#endif // _WIN32
