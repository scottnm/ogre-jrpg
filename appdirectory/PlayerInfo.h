#ifndef __PLAYER_INFO_H__
#define __PLAYER_INFO_H__

#include <string>

struct PlayerInfo {
    PlayerInfo(const std::string& n, const std::string& img, int hp, int sp,
            int dmg, int armor, float accuracy)
        : name(n), img(img), health(hp), healthMax(hp), specialPoints(sp),
          specialPointsMax(sp), damage(dmg), baseDamage(dmg), armor(armor),
          baseArmor(armor), accuracy(accuracy), baseAccuracy(accuracy) {}

    std::string name;
    std::string img;

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

    void reset(void) {
        health = healthMax;
        specialPoints = specialPointsMax;
        damage = baseDamage;
        armor = baseArmor;
        accuracy = baseAccuracy;
    }
};

#endif // __PLAYER_INFO_H__
