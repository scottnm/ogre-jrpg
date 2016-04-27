#ifndef __ITEM_H__
#define __ITEM_H__

#include <string>

#include "Player.h"

class Item {
public:
	Item(std::string name, bool isOffensive, int d_health, int d_armor, int d_damage, int d_special, float d_accuracy);
	void use(Player& target);

	std::string name;
	bool isOffensive;
	int delta_health;
	int delta_armor;
	int delta_damage;
	int delta_special;
	float delta_accuracy;
};

#endif // __INVENTORY_H__