#include "PlayerBank.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

PlayerBank::PlayerBank() {
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
}

PlayerBank::~PlayerBank() {
    // iterate over chunkbank and free all of the chunks
    for(auto pair : mPlayerBank) {
        delete pair.second;
    }
     mPlayerBank.clear();
}

void PlayerBank::loadPlayerDatabase(const std::string& _fn) {
    // based on xml parser found here http://www.boost.org/doc/libs/1_42_0/doc/html/boost_propertytree/tutorial.html

    using boost::property_tree::ptree;
    ptree pt;

    // Load the XML file into the property tree. If reading fails
    // (cannot open file, parse error), an exception is thrown.
    read_xml(_fn, pt);

    //std::string srcdir = pt.get<std::string>("characterDatabase.sourcedir") + "/";

    // Iterate over the debug.modules section and store all found
    // modules in the m_modules set. The get_child() function
    // returns a reference to the child at the specified path; if
    // there is no such child, it throws. Property tree iterators
    // are models of BidirectionalIterator.
    BOOST_FOREACH(ptree::value_type &v, pt.get_child("characterDatabase.characters")) {
        std::string name = v.second.get<std::string>("name");
        int health = v.second.get<int>("health");
        int special = v.second.get<int>("special");
        int damage = v.second.get<int>("damage");
        int armor = v.second.get<int>("armor");
        float accuracy = v.second.get<float>("accuracy");
        PlayerInfo* player = new PlayerInfo(name, health, special, damage, armor, accuracy);
        mPlayerBank.emplace(id, player);
    }
}