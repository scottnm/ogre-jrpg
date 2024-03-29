#include "PlayerBank.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#include <iostream> // cerr
#include <cstdlib>

PlayerBank::PlayerBank() {
    mMeshSpecBank.loadMeshSpecDatabase("assets/meshes/MeshSpecs.xml");
}

void PlayerBank::loadPlayerDatabase(const std::string& _fn) {
    // based on xml parser found here
    // http://www.boost.org/doc/libs/1_42_0/doc/html/boost_propertytree/tutorial.html

    using boost::property_tree::ptree;
    ptree pt;
    read_xml(_fn, pt);
    
    BOOST_FOREACH(ptree::value_type &v,
            pt.get_child("characterDatabase.characters")) {
        std::string name = v.second.get<std::string>("name");
        std::string img = v.second.get<std::string>("img");
        std::string meshname = v.second.get<std::string>("mesh");
        std::string materialname = v.second.get<std::string>("material");
        const MeshSpec& meshspec = mMeshSpecBank.getMeshSpec(meshname);
        int health = v.second.get<int>("health");
        int special = v.second.get<int>("special");
        int damage = v.second.get<int>("damage");
        int armor = v.second.get<int>("armor");
        float accuracy = v.second.get<float>("accuracy");
        mCharacterBank.emplace(name, PlayerInfo(name, img, meshspec, materialname,
                    health, special, damage, armor, accuracy));
    }

    for (auto pair : mCharacterBank) {
        std::string info;
        info += pair.second.name + " | "; 
        info += "He(" + std::to_string(pair.second.health)   + ") ";
        info += "AP(" + std::to_string(pair.second.specialPoints)  + ") ";
        info += "Da(" + std::to_string(pair.second.damage)   + ") ";
        info += "Ar(" + std::to_string(pair.second.armor)    + ") ";
        info += "Ac(" + std::to_string(pair.second.accuracy) + ") ";
        std::cout << info << std::endl;
    }
}

const PlayerInfo& PlayerBank::getPlayerInfo(const std::string& _n) {
    auto pair = mCharacterBank.find(_n);
    if (pair == mCharacterBank.end()) {
        std::cerr << "\n***** Could not find character <" << _n << ">";
        std::cerr << "\b***** Terminating.... " << std::endl;
        exit(EXIT_FAILURE);
    }

    return pair->second;
}
