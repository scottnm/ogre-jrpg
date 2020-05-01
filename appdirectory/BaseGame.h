#pragma once

/*
-----------------------------------------------------------------------------
Filename:    BaseGame.h
-----------------------------------------------------------------------------

This source file was adopted from the Ogre Wiki's MinimalOgre.h
http://www.ogre3d.org/tikiwiki/tiki-index.php?page=MinimalOgre-h&structure=Development
-----------------------------------------------------------------------------
*/
#include "GUISystem.h"
#include "RenderingEngine.h"
#include "Plane.h"
#include "Player.h"
#include "Inventory.h"

#include "SoundBank.h"
#include "PlayerBank.h"

#include <OgreParticleSystem.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkCameraMan.h>
#include <SdkTrays.h>

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

#include <CEGUI/CEGUI.h>

#include <vector>
#include <string>

class BaseGame : public Ogre::FrameListener
{
public:
    BaseGame(RenderingEngine* renderer, GUISystem* gui, PlayerBank* playerBank);
    virtual ~BaseGame(void);
    virtual bool go(std::vector<std::string> v) = 0;
    virtual bool isShutDown(void) { return mShutDown; }

    RenderingEngine* mRenderer;
    GUISystem* mGUI;
    PlayerBank* mPlayerBank;
    bool mShutDown;

    // Ogre Bites
    bool mCursorWasVisible;

    // in game objects
    Ogre::SceneNode* mRoomRoot;
    Ogre::Light* mMainLight;
    static Ogre::String mainLightName;
    Plane* ground;

    std::vector<Player*> myParty;
    std::vector<Player*> myPartyAlive;
    std::vector<Player*> enemyParty;
    std::vector<Player*> enemyPartyAlive;
    std::vector<Player*> myPartyWaiting;
    std::vector<Ogre::Vector3> partyCameraPositions;
    std::vector<Ogre::Vector3> enemyCameraPositions;
    Ogre::Vector3 cameraInitialPosition;
    Ogre::Vector3 cameraInitialLookAt;
    Inventory mInventory;

    std::vector<std::string> windowNames;

    // listeners
    virtual bool keyPressed( const OIS::KeyEvent &arg );

    // setup
    virtual void createScene(std::vector<std::string>) = 0;
    virtual void destroyScene(void) = 0;
    void initGUI(void);

    // gui callbacks
    bool guiCbQuit(const CEGUI::EventArgs& e);
};
