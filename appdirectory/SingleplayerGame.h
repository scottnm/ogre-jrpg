/*
-----------------------------------------------------------------------------
Filename:    SingleplayerGame.h
-----------------------------------------------------------------------------
 
This source file was adopted from the Ogre Wiki's MinimalOgre.h
http://www.ogre3d.org/tikiwiki/tiki-index.php?page=MinimalOgre-h&structure=Development
-----------------------------------------------------------------------------
*/
#ifndef __SINGLE_PLAYER_GAME_H__
#define __SINGLE_PLAYER_GAME_H__
 
#include "BaseGame.h"
#include "HUD.h"

class SingleplayerGame : public BaseGame,
                         public HUDListener
{
public:
    SingleplayerGame(RenderingEngine* renderer, GUISystem* gui, SoundBank* soundBank);
    virtual ~SingleplayerGame(void);
    bool go(void);

private:
    void createScene(void);
    void destroyScene(void);
    void initGUI(void);
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);

    // gui
    HUD* mHUD;
    void onHUDPhysicalSelect();
    void onHUDSpecialSelect();
    void onHUDItemSelect();
    void onHUDGuardSelect();


    /*
    bool guiCbStart(const CEGUI::EventArgs& e);
    bool guiCbResume(const CEGUI::EventArgs& e);
    bool guiCbQuit(const CEGUI::EventArgs& e);
    */
};
 
#endif // __SINGLE_PLAYER_GAME_H__
