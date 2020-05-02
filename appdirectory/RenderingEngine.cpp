#ifndef _WIN32 // TODO: get to compile

#include "RenderingEngine.h"

bool RenderingEngine::instanceCreated = false;
RenderingEngine* RenderingEngine::onlyInstance = nullptr;
bool RenderingEngine::engineErr = false;

RenderingEngine* RenderingEngine::getSingleton() {
    if (!instanceCreated) {
        onlyInstance = new RenderingEngine;
        if (engineErr) {
            delete onlyInstance;
            return nullptr;
        } else {
            instanceCreated = true;
        }
    }
    return onlyInstance;
}

void RenderingEngine::destroySingleton() {
    if (instanceCreated) {
        delete onlyInstance;
        instanceCreated = false;
        engineErr = false;
    }
}

RenderingEngine::RenderingEngine()
    : mRoot(nullptr),
      mSceneManager(nullptr),
      mWindow(nullptr),
      mOverlaySystem(nullptr),
      mCamera(nullptr),
      resourcesConfig(Ogre::StringUtil::BLANK),
      pluginsConfig(Ogre::StringUtil::BLANK) {
    #ifdef _DEBUG
    resourcesConfig = "resources_d.cfg";
    pluginsConfig = "plugins_d.cfg";
    #else
    resourcesConfig = "resources.cfg";
    pluginsConfig = "plugins.cfg";
    #endif

    mRoot = new Ogre::Root(pluginsConfig);

    // setup resources and load resource paths
    Ogre::ConfigFile cf;
    cf.load(resourcesConfig);
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while(seci.hasMoreElements()) {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap& settings = *seci.getNext();
        for(auto i : settings) {
            typeName = i.first;
            archName = i.second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName);
        }
    }

    // resource the config settings or show a dialog box to setup new settings
    // if both fail then return early
    if (!mRoot->restoreConfig() && !mRoot->showConfigDialog()) {
        engineErr = true;
        return;
    }
    mWindow = mRoot->initialise(true, "WINDOW");

    mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC);
    mOverlaySystem = new Ogre::OverlaySystem();
    mSceneManager->addRenderQueueListener(mOverlaySystem);

    mCamera = mSceneManager->createCamera("PlayerCam");
    mCamera->setNearClipDistance(0.01);

    auto viewport = mWindow->addViewport(mCamera);
    viewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

    mCamera->setAspectRatio(
            Ogre::Real(viewport->getActualWidth()) /
            Ogre::Real(viewport->getActualHeight()));

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    engineErr = false;
}

RenderingEngine::~RenderingEngine(void) {
    if (mOverlaySystem) delete mOverlaySystem;
    if (mRoot) delete mRoot;
}

bool RenderingEngine::isRenderWindow(Ogre::RenderWindow* rw) {
    return rw == mWindow;
}

void RenderingEngine::getRenderMetrics(unsigned int& w, unsigned int& h,
        unsigned int& d, int& l, int& t) {
    mWindow->getMetrics(w, h, d, l, t);
}

void RenderingEngine::addFrameListener(Ogre::FrameListener* fl) {
    mRoot->addFrameListener(fl);
}

void RenderingEngine::addWindowEventListener(Ogre::WindowEventListener* wel) {
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, wel);
}

void RenderingEngine::removeWindowEventListener(Ogre::WindowEventListener* wel) {
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, wel);
}

void RenderingEngine::startRendering() {
    mRoot->startRendering();
}

bool RenderingEngine::isWindowClosed() {
    return mWindow->isClosed();
}

#endif // _WIN32
