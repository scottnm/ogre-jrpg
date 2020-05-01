#pragma once

/*
-----------------------------------------------------------------------------
Filename:    SingleplayerGame.h
-----------------------------------------------------------------------------

This source file was adopted from the Ogre Wiki's MinimalOgre.h
http://www.ogre3d.org/tikiwiki/tiki-index.php?page=MinimalOgre-h&structure=Development
-----------------------------------------------------------------------------
*/
#include "BaseGame.h"
#include "HUD.h"
#include "HUDListener.h"
#include <SDL_mixer.h>

class SingleplayerGame : public BaseGame,
                         public HUDListener
{
public:
    SingleplayerGame(RenderingEngine* renderer, GUISystem* gui, PlayerBank* playerBank, SoundBank* soundBank);
    virtual ~SingleplayerGame(void);
    bool go(std::vector<std::string> v);


private:
    void createScene(std::vector<std::string> v);
    void destroyScene(void);
    void initGUI(void);
    bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual bool keyPressed(const OIS::KeyEvent &arg);

    void partyReset(std::vector<Player*>& party);
    void clearDeadCharacters(void);

    void onHUDPhysicalSelect(Player* attacker, Player* target);
    void onHUDSpecialSelect(Player* attacker, Player* target);
    void onHUDItemSelect(Player* user, Player* target);
    void onHUDGuardSelect(Player* user);
    void onHUDCycleCharacter(void);
    void onHUDOptionSelect();
    void onHUDNavigation();
    void onHUDPlayAgain();
    void onHUDPlayNext();
    void onHUDQuit();

    bool mGameOver;
    bool mAttackRunning;
    HUD* mHUD;
    SoundBank* mSoundBank;
    bool playerTurn;
    unsigned int activeEnemy;
    int mWaveCnt;

    time_t timer;

    Mix_Music* mMusicTrack;
};
