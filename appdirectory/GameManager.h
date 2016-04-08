#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "BaseGame.h"
#include "SingleplayerGame.h"
#include "GUISystem.h"
#include "RenderingEngine.h"

#include <OgreFrameListener.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

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

    bool guiCbInitSinglePlayer(const CEGUI::EventArgs& e);
    bool guiCbQuit(const CEGUI::EventArgs& e);

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
    BaseGame* mGame;
    bool mShutDown;
    
    // Sound System goes here

    // OIS Input devices
    OIS::InputManager* mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;
    OgreBites::InputContext mInputContext;
};

#endif // __GAME_MANAGER_H__
