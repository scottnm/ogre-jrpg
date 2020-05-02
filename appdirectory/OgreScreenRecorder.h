#ifndef _WIN32 // TODO: get to compile

#pragma once

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

#endif // _WIN32