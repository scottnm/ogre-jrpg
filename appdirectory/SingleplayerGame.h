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
#include "HUDListener.h"
#include "SoundController.h"
#include <SDL_mixer.h>

class SingleplayerGame : public BaseGame,
                         public HUDListener
{
public:
    SingleplayerGame(RenderingEngine* renderer, GUISystem* gui, PlayerBank* playerBank, SoundBank* soundBank);
    virtual ~SingleplayerGame(void);
    bool go(void);

private:
    void createScene(void);
    void destroyScene(void);
    void initGUI(void);
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual bool keyPressed(const OIS::KeyEvent &arg);

    void onRoundOver(void);

    void onHUDPhysicalSelect(Player* attacker, Player* target);
    void onHUDSpecialSelect(Player* attacker, Player* target);
    void onHUDItemSelect(Player* user, Player* target);
    void onHUDGuardSelect(Player* user);
    void onHUDPlayAgain();
    void onHUDQuit();

    bool mGameOver;
    HUD* mHUD;
    SoundController mSoundController;
    bool playerTurn;
    unsigned int activeEnemy;

    Mix_Music* mMusicTrack;
};
 
#endif // __SINGLE_PLAYER_GAME_H__
