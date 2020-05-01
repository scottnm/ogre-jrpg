#pragma once

#include <string>
#include <unordered_map>

#include "PlayerInfo.h"
#include "MeshSpecBank.h"

class PlayerBank {
public:
    PlayerBank(void);

    /**
     * load the sounds specified by the xml audio database file
     * @param xmlDbFilename - the name of the xml audio database file
     */
    void loadPlayerDatabase(const std::string& xmlDbFilename);
    const PlayerInfo& getPlayerInfo(const std::string& name);

private:
    MeshSpecBank mMeshSpecBank;
    std::unordered_map<std::string, PlayerInfo> mCharacterBank;
};
