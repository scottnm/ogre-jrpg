#ifndef _WIN32 // TODO: get to compile

#pragma once

#include <string>

#include "Player.h"

class Item {
public:
	Item(std::string name, std::string description, bool isOffensive, int d_health, int d_armor, int d_damage, int d_special, float d_accuracy);
	void use(Player& target);

	std::string name;
	std::string description;
	bool isOffensive;
	int delta_health;
	int delta_armor;
	int delta_damage;
	int delta_special;
	float delta_accuracy;
};

#endif // _WIN32