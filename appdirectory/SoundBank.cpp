#include "SoundBank.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

SoundBank::SoundBank() {
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
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
            std::cout << "Audio failed to load: " << fname;
            std::cout << "\t::\t" << Mix_GetError() << std::endl;
        }
        else {
            mChunkBank.emplace(id, chunk);
        }
    }
}

#define FIRST_FREE_CHANNEL -1
#define NO_LOOPS 0
void SoundBank::play(const std::string& id) {
    Mix_PlayChannel(FIRST_FREE_CHANNEL, mChunkBank.find(id)->second, NO_LOOPS);
}
