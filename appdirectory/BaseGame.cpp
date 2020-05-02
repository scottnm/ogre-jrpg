#ifndef _WIN32 // TODO: get to compile

/*
-----------------------------------------------------------------------------
Filename:    BaseGame.cpp
-----------------------------------------------------------------------------

This source file is adopted from MinimalOgre.cpp
http://www.ogre3d.org/tikiwiki/tiki-index.php?page=MinimalOgre-cpp
-----------------------------------------------------------------------------
*/

#include "BaseGame.h"
#include <string>

Ogre::String BaseGame::mainLightName = "MainLight";

BaseGame::BaseGame(RenderingEngine* renderer, GUISystem* gui,
                   PlayerBank* playerBank)
    : mRenderer(renderer),
    mGUI(gui),
    mPlayerBank(playerBank),
    mShutDown(false),
    mCursorWasVisible(false),
    mRoomRoot(nullptr),
    mInventory("assets/item/inventory.xml") {
}

BaseGame::~BaseGame(void) {
    for(std::string n : windowNames) {
        mGUI->destroyWindowGroup(n);
    }
    mRenderer->mSceneManager->getRootSceneNode()->removeAndDestroyAllChildren();
}

void BaseGame::initGUI() {
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* baseGUI = wmgr.createWindow("DefaultWindow", "BaseGameBaseWindow");
    CEGUI::Window* quit = wmgr.createWindow("WindowsLook/Button", "BaseGameQuitButton");
    quit->setText("Quit");
    quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
    baseGUI->addChild(quit);
    quit->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&BaseGame::guiCbQuit, this));
    mGUI->addAndSetWindowGroup("BaseGUI", baseGUI);
    windowNames.push_back("BaseGUI");
}


bool BaseGame::keyPressed( const OIS::KeyEvent &arg ) {
    switch(arg.key) {
        case OIS::KC_ESCAPE:
            mShutDown = true;
            break;
        default:
            break;
    }
    return true;
}

bool BaseGame::guiCbQuit(const CEGUI::EventArgs& e) {
    mShutDown = true;
    return true;
}


#endif // _WIN32
