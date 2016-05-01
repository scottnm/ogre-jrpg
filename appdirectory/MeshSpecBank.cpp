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
        AnimationSpec as(
            v.second.get<std::string>("animationSpec.idle.state-name"),
            v.second.get<float>("animationSpec.idle.speed-scale"),
            v.second.get<std::string>("animationSpec.physical.state-name"),
            v.second.get<float>("animationSpec.physical.speed-scale"),
            v.second.get<std::string>("animationSpec.special.state-name"),
            v.second.get<float>("animationSpec.special.speed-scale"),
            v.second.get<std::string>("animationSpec.item.state-name"),
            v.second.get<float>("animationSpec.item.speed-scale"),
            v.second.get<std::string>("animationSpec.guard.state-name"),
            v.second.get<float>("animationSpec.guard.speed-scale"),
            v.second.get<std::string>("animationSpec.death.state-name"),
            v.second.get<float>("animationSpec.death.speed-scale"));
        mSpecBank.emplace(name, MeshSpec(name, as));
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
