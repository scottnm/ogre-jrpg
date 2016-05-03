#include "SoundBank.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <cstdlib>

#include <cassert>

SoundBank::SoundBank() {
    auto initted = Mix_Init(MIX_INIT_OGG);
    assert(initted == MIX_INIT_OGG);
    Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 1024);
}

SoundBank::~SoundBank() {
    // iterate over chunkbank and free all of the chunks
    for(auto pair : mChunkBank) {
        Mix_FreeChunk(pair.second);
    }
    mChunkBank.clear();
}

void SoundBank::loadAudioDatabase(const std::string& _fn) {
    // based on xml parser found here http://www.boost.org/doc/libs/1_42_0/doc/html/boost_propertytree/tutorial.html

    using boost::property_tree::ptree;
    ptree pt;
    read_xml(_fn, pt);

    std::string srcdir = pt.get<std::string>("audioDatabase.sourcedir") + "/";
    BOOST_FOREACH(ptree::value_type &v, pt.get_child("audioDatabase.sources")) {
        std::string id = v.second.get<std::string>("id");
        std::string fname = srcdir + v.second.get<std::string>("filename");
        Mix_Chunk* chunk = Mix_LoadWAV(fname.c_str());
        if (chunk == nullptr) {
            std::cerr << "\n***** Audio failed to load: " << fname;
            std::cerr << "\n***** " << Mix_GetError();
            std::cerr << "\n***** Terminating...\n" << std::endl;
            exit(EXIT_FAILURE);
        }
        else {
            mChunkBank.emplace(id, chunk);
        }
    }
}

#define FIRST_FREE_CHANNEL -1
#define NO_LOOPS 0
void SoundBank::play(const std::string& id) {
    auto chunkpair = mChunkBank.find(id);
    if (chunkpair == mChunkBank.end()) {
        std::cerr << "\n***** Could not find sound <" << id << ">\n***** Terminating..." << std::endl;
        exit(EXIT_FAILURE);
    }
    Mix_PlayChannel(FIRST_FREE_CHANNEL, mChunkBank.find(id)->second, NO_LOOPS);
}
