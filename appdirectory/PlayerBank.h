#ifndef __PLAYERBANK_H__
#define __PLAYERBANK_H__

#include <string>
#include <unordered_map>

class PlayerBank {
public:
    PlayerBank();
    ~PlayerBank();

    /**
     * load the sounds specified by the xml audio database file
     * @param xmlDbFilename - the name of the xml audio database file
     */
    void loadPlayerDatabase(const std::string& xmlDbFilename);

private:
    std::unordered_map<std::string, PlayerInfo*> mPlayerBank;
};

#endif // __PLAYERBANK_H__
