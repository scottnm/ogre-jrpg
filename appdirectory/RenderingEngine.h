#ifndef __RENDERING_ENGINE_H__
#define __RENDERING_ENGINE_H__

#include <OGRE/Overlay/OgreOverlayPrerequisites.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <OgreCamera.h>
#include <OgreConfigFile.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreViewport.h>
#include <OgreWindowEventUtilities.h>

#include <OISInputManager.h>

class RenderingEngine
{
public:
    Ogre::Root* mRoot;
    Ogre::SceneManager* mSceneManager;
    Ogre::RenderWindow* mWindow;
    Ogre::OverlaySystem* mOverlaySystem;
    Ogre::Camera* mCamera;
    Ogre::String resourcesConfig;
    Ogre::String pluginsConfig;

    static RenderingEngine* getSingleton(void);
    static void destroySingleton(void);

    void getRenderMetrics(unsigned int& width, unsigned int& height,
            unsigned int& depth, int& left, int& top);
    bool isRenderWindow(Ogre::RenderWindow* rw);
    void addFrameListener(Ogre::FrameListener* fl);
    void addWindowEventListener(Ogre::WindowEventListener* wel);
    void removeWindowEventListener(Ogre::WindowEventListener* wel);
    void startRendering(void);
    bool isWindowClosed(void);

private:
    static bool instanceCreated;
    static RenderingEngine* onlyInstance;
    static bool engineErr;

    RenderingEngine(void);
    ~RenderingEngine(void);
};

#endif // __RENDERING_ENGINE_H__
