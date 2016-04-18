#ifndef __PLAYERBANK_H__
#define __PLAYERBANK_H__

#include <string>
#include <unordered_map>

#include "PlayerInfo.h"

class PlayerBank {
public:
    PlayerBank();
    ~PlayerBank();

    /**
     * load the sounds specified by the xml audio database file
     * @param xmlDbFilename - the name of the xml audio database file
     */
    void loadPlayerDatabase(const std::string& xmlDbFilename);
    const PlayerInfo& getPlayerInfo(const std::string& name);

private:
    std::unordered_map<std::string, PlayerInfo*> mCharacterBank;
};

#endif // __PLAYERBANK_H__
