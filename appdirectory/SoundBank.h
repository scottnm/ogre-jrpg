#ifndef _WIN32 // TODO: get to compile

#pragma once

#include <SDL.h>
#include <SDL_mixer.h>

#include <string>
#include <unordered_map>

class SoundBank {
public:
    SoundBank();
    ~SoundBank();

    /**
     * load the sounds specified by the xml audio database file
     * @param xmlDbFilename - the name of the xml audio database file
     */
    void loadAudioDatabase(const std::string& xmlDbFilename);

    /**
     * play a sound specified by the given id
     * @param id - Id of the sound file in the xml audio database file
     */
    void play(const std::string& id);
private:
    std::unordered_map<std::string, Mix_Chunk*> mChunkBank;
};

#endif // _WIN32
