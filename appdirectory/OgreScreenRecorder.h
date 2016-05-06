#ifndef __OGRE_SCREEN_RECORDER__
#define __OGRE_SCREEN_RECORDER__

#include <OgreFrameListener.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>

#include <iostream>

class OgreScreenRecorder : public Ogre::FrameListener
{
public:
    OgreScreenRecorder(Ogre::Root& r, Ogre::RenderWindow& w);
    ~OgreScreenRecorder();

private:
    bool frameEnded(const Ogre::FrameEvent& evt);

    Ogre::Root& mRoot;
    Ogre::RenderWindow& mWindow;
    int cnt;
    char filename_buf[25];
    Ogre::Real timer;
    const Ogre::Real limit;
};

#endif // __OGRE_SCREEN_RECORDER__
