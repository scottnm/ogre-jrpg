#pragma once

#include <string>
#include <unordered_map>

#include "MeshSpec.h"

class MeshSpecBank {
public:
    /**
     * load the sounds specified by the xml audio database file
     * @param xmlDbFilename - the name of the xml audio database file
     */
    void loadMeshSpecDatabase(const std::string& xmlDbFilename);
    const MeshSpec& getMeshSpec(const std::string& name);

private:
    std::unordered_map<std::string, MeshSpec> mSpecBank;
};
