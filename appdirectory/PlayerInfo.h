#ifndef __PLAYER_INFO_H__
#define __PLAYER_INFO_H__

#include <string>
#include "MeshSpec.h"

struct PlayerInfo {
    PlayerInfo(const std::string& name, const std::string& img, const MeshSpec& mesh,
            const std::string& mat, int hp, int sp, int dmg, int armor, float accuracy, float speed)
        : name(name), img(img), mesh(mesh), material(mat), health(hp), healthMax(hp),
          specialPoints(sp), specialPointsMax(sp), damage(dmg), baseDamage(dmg), armor(armor),
          baseArmor(armor), accuracy(accuracy), baseAccuracy(accuracy), speed(speed), baseSpeed(speed) {}

    const std::string name;
    const std::string img;
    const MeshSpec mesh;
    const std::string material;

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

    float speed;
    const float baseSpeed;

    void reset(void) {
        health = healthMax;
        specialPoints = specialPointsMax;
        damage = baseDamage;
        armor = baseArmor;
        accuracy = baseAccuracy;
    }
};

#endif // __PLAYER_INFO_H__
