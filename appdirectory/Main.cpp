#ifndef _WIN32 // TODO: get to compile

#include "OgreScreenRecorder.h"
#include "RenderingEngine.h"
#include "GameManager.h"
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {

        OgreScreenRecorder* rec = nullptr;
        if(argc > 1 && std::string(argv[1]) == std::string("--rec")) {
            auto re = RenderingEngine::getSingleton();
            rec = new OgreScreenRecorder(*re->mRoot, *re->mWindow);
        }
        srand(time(NULL));
        // Create application object
        try {
            GameManager manager;
            manager.runGame();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        if (rec) delete rec;
        return 0;
    }

#ifdef __cplusplus
}
#endif

#endif // _WIN32
