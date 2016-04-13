/*
-----------------------------------------------------------------------------
Filename:    BaseGame.h
-----------------------------------------------------------------------------
 
This source file was adopted from the Ogre Wiki's MinimalOgre.h
http://www.ogre3d.org/tikiwiki/tiki-index.php?page=MinimalOgre-h&structure=Development
-----------------------------------------------------------------------------
*/
#ifndef __BASE_GAME_H__
#define __BASE_GAME_H__
 
#include "GameObject.h"
#include "GUISystem.h"
#include "RenderingEngine.h"
#include "Plane.h"
#include "Player.h"

#include "OgreParticleSystem.h"
#include "OgreParticleSystemManager.h"
#include "OgreParticle.h"

#include <vector>

#include "SoundBank.h"

#include <SDL/SDL.h>
#include <SDL/SDL_net.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
 
#include <SdkCameraMan.h>
#include <SdkTrays.h>

#include <CEGUI/CEGUI.h>
 
class BaseGame : public Ogre::FrameListener
{
public:
    BaseGame(RenderingEngine* renderer, GUISystem* gui, SoundBank* soundBank);
    virtual ~BaseGame(void);
    virtual bool go(void) = 0;
    virtual bool isShutDown(void) { return mShutDown; }

    RenderingEngine* mRenderer;
    GUISystem* mGUI;
    SoundBank* mSoundBank;
    bool mShutDown;

    // Ogre Bites
    bool mCursorWasVisible;

    // in game objects
    Ogre::SceneNode* mRoomRoot;
    Ogre::Light* mMainLight;
    static Ogre::String mainLightName;
    Plane* ground;
    std::vector<Player*> player_list;

    std::vector<std::string> windowNames;

    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt) = 0;
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg ) = 0;
    virtual bool mouseMoved( const OIS::MouseEvent &arg ) = 0;
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) = 0;
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) = 0;

    // setup
    virtual void createScene(void) = 0;
    virtual void destroyScene(void) = 0;
    void initGUI(void);

    // gui callbacks
    bool guiCbQuit(const CEGUI::EventArgs& e);
};
 
#endif // __BASE_GAME_H__
