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
        std::string n = v.second.get<std::string>("name");
        mSpecBank.emplace(n, MeshSpec(n, n, n, n, n, n, n));
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
