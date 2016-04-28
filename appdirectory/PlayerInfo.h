#ifndef __PLAYER_INFO_H__
#define __PLAYER_INFO_H__

#include <string>

enum class MeshAnchor {
    BOTTOM = 0,
    MIDDLE,
    TOP
};

struct MeshInfo {
    MeshInfo(const std::string& n, const MeshAnchor a) : name(n), anchorPoint(a) {}
    const std::string name;
    const MeshAnchor anchorPoint;
};

struct PlayerInfo {
    PlayerInfo(const std::string& name, const std::string& img, const std::string& mesh,
            const MeshAnchor anchor, int hp, int sp, int dmg, int armor, float accuracy)
        : name(name), img(img), meshInfo(mesh, anchor), health(hp), healthMax(hp), specialPoints(sp),
          specialPointsMax(sp), damage(dmg), baseDamage(dmg), armor(armor),
          baseArmor(armor), accuracy(accuracy), baseAccuracy(accuracy) {}

    const std::string name;
    const std::string img;

    const MeshInfo meshInfo;

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
