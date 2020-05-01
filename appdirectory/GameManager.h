#pragma once

#include "BaseGame.h"
#include "SingleplayerGame.h"
#include "GUISystem.h"
#include "RenderingEngine.h"
#include "SoundBank.h"
#include "PlayerBank.h"

#include <OgreFrameListener.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include <set>

class GameManager : public Ogre::FrameListener,
                    public Ogre::WindowEventListener,
                    public OIS::KeyListener,
                    public OIS::MouseListener {
public:
    GameManager(void);
    ~GameManager(void);
    bool runGame(void);

private:
    void closeGame(void);

    bool guiCbClickFrame(const CEGUI::EventArgs& e);
    bool guiCbConfirmButton(const CEGUI::EventArgs& e);
    bool guiCbQuit(const CEGUI::EventArgs& e);
    bool guiCbReturnToSelectScreen(const CEGUI::EventArgs& e);
    bool guiCbShowInstructions(const CEGUI::EventArgs& e);

    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);

    RenderingEngine* mRenderer;
    GUISystem* mGUI;
    SoundBank mSoundBank;
    PlayerBank mPlayerBank;
    BaseGame* mGame;
    bool mShutDown;
    std::vector<PlayerInfo> possible_players;
    std::vector<CEGUI::Window*> frames;
    std::set<int> party;

    CEGUI::Window* mRoot;
    CEGUI::Window* charRoot;
    CEGUI::Window* instructionRoot;

    // Sound System goes here

    // OIS Input devices
    OIS::InputManager* mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;
    OgreBites::InputContext mInputContext;
};
