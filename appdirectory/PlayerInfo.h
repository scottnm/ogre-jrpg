#ifndef __PLAYER_INFO_H__
#define __PLAYER_INFO_H__

#include <string>

struct PlayerInfo {
    PlayerInfo(const std::string& n, int hp, int sp, int dmg, int armor,
            float accuracy)
        : name(n), health(hp), healthMax(hp), specialPoints(sp),
          specialPointsMax(sp), damage(dmg), baseDamage(dmg), armor(armor),
          baseArmor(armor), accuracy(accuracy), baseAccuracy(accuracy) {}

    std::string name;

    int health;
    const int healthMax;

    int specialPoints;
    const int specialPointsMax;

    int damage;
    const int baseDamage;

    int armor;
    const int baseArmor;

    float accuracy;
    const float baseAccuracy;
};

#endif // __PLAYER_INFO_H__
