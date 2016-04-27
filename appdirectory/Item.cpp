#include "Item.h"

Item::Item(std::string name, bool isOffensive, int d_health, int d_armor,
	       int d_damage, int d_special, float d_accuracy) 
	: name(name), isOffensive(isOffensive), delta_health(d_health), delta_armor(d_armor),
	  delta_damage(d_damage), delta_special(d_special), delta_accuracy(d_accuracy) {}

void Item::use(Player& target) {
    PlayerInfo& info = target.info();
	info.health += delta_health;
	info.armor += delta_armor;
	info.damage += delta_damage;
	info.specialPoints += delta_special;
	info.accuracy += delta_accuracy;
}
