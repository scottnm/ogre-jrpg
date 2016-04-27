#include "Item.h"

Item::Item(std::string name, bool isOffensive, int d_health, int d_armor,
	       int d_damage, int d_special, float d_accuracy) 
	: name(name), isOffensive(isOffensive), delta_health(d_health), delta_armor(d_armor),
	  delta_damage(d_damage), delta_special(d_special), delta_accuracy(d_accuracy) {}

void Item::use(Player& target) {
    PlayerInfo& info = target.info();
    
    int health = std::max(info.health + delta_health, 0);
    info.health = std::min(health, info.healthMax);

    int special = std::max(info.specialPoints + delta_special, 0);
    info.specialPoints = std::min(special, info.specialPointsMax);
	
	info.armor += std::max(info.armor + delta_health, 0);
	info.damage += std::max(info.damage + delta_damage, 0);
	info.accuracy += std::max(info.accuracy + delta_accuracy, 0.0f);
}
