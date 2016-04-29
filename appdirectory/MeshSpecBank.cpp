#include "MeshSpecBank.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#include <iostream> // cerr
#include <cstdlib>

void MeshSpecBank::loadMeshSpecDatabase(const std::string& _fn) {
    // based on xml parser found here
    // http://www.boost.org/doc/libs/1_42_0/doc/html/boost_propertytree/tutorial.html

    using boost::property_tree::ptree;
    ptree pt;
    read_xml(_fn, pt);
    
    BOOST_FOREACH(ptree::value_type &v,
            pt.get_child("meshSpecDatabase")) {
        std::cout << "!!!" << std::endl;
        std::string name = v.second.get<std::string>("name");
        std::string idle = v.second.get<std::string>("animationSpec.idle");
        std::string phys = v.second.get<std::string>("animationSpec.physical");
        std::string special = v.second.get<std::string>("animationSpec.special");
        std::string item = v.second.get<std::string>("animationSpec.item");
        std::string guard = v.second.get<std::string>("animationSpec.guard");
        std::string death = v.second.get<std::string>("animationSpec.death");
        mSpecBank.emplace(name, MeshSpec(name, idle, phys, special, item, guard, death));
    }

    for(auto specPair : mSpecBank) {
        std::cout << "spec: " << specPair.second.name << std::endl;
    }
}

const MeshSpec& MeshSpecBank::getMeshSpec(const std::string& _n) {
    auto pair = mSpecBank.find(_n);
    if (pair == mSpecBank.end()) {
        std::cerr << "\n***** Could not find meshspec <" << _n << ">";
        std::cerr << "\n***** Terminating.... " << std::endl;
        exit(EXIT_FAILURE);
    }

    return pair->second;
}
