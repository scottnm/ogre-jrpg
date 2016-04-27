/*
-----------------------------------------------------------------------------
Filename:    SingleplayerGame.cpp
-----------------------------------------------------------------------------
 
This source file is adopted from MinimalOgre.cpp
http://www.ogre3d.org/tikiwiki/tiki-index.php?page=MinimalOgre-cpp
-----------------------------------------------------------------------------
*/

#include "ParticleType.h"
#include "SingleplayerGame.h"
#include <string>
#include <algorithm>

SingleplayerGame::SingleplayerGame(RenderingEngine* renderer, GUISystem* gui,
    PlayerBank* playerBank, SoundBank* soundBank)
    : BaseGame(renderer, gui, playerBank), mGameOver(false), mSoundController(soundBank),
      playerTurn(true), activeEnemy(0) {
}

SingleplayerGame::~SingleplayerGame(void) {
    Mix_FadeOutMusic(1000);
    Mix_FreeMusic(mMusicTrack);
    destroyScene();
    if (mHUD) delete mHUD;
    mRenderer->mRoot->removeFrameListener(this);
}

void SingleplayerGame::createScene(void){
    auto scnMgr = mRenderer->mSceneManager;
    auto root = scnMgr->getRootSceneNode();
    Ogre::Camera* camera = mRenderer->mCamera;
    mRoomRoot = root->createChildSceneNode();
    scnMgr->setSkyBox(true, "Examples/MorningSkyBox", 5000, true);


    // Set ambient light
    scnMgr->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));
    scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
 
    // Create a light
    mMainLight = scnMgr->createLight(BaseGame::mainLightName);
    mMainLight->setPosition(0, 500, 0);
    mMainLight->setType(Ogre::Light::LT_POINT);
    mMainLight->setDiffuseColour(.5, .5, .5);

    // Create ground plane
    ground = new Plane(scnMgr, mRoomRoot);

    // Add test objects
    Player* p = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("Cannibal Corpse"),
            Ogre::Vector3(500, 0, 200));
    myParty.push_back(p);

    Player* p2 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("Spooky Boo"),
            Ogre::Vector3(500, 0, 0));
    myParty.push_back(p2);

    Player* p3 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("Rattlebones"),
            Ogre::Vector3(500, 0, -200));
    myParty.push_back(p3);
    

    Player* p4 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("Mecha-Scoot"),
            Ogre::Vector3(-500, 0, 200));
    enemyParty.push_back(p4);

    Player* p5 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("SSJVirginia"),
            Ogre::Vector3(-500, 0, 0));
    enemyParty.push_back(p5);

    Player* p6 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("Metal Scoot"),
            Ogre::Vector3(-500, 0, -200));
    enemyParty.push_back(p6);

    // Set Camera Position
    camera->setPosition(Ogre::Vector3(1000, 450, 1000));
    camera->lookAt(Ogre::Vector3(0, 0, 0));
}

void SingleplayerGame::destroyScene(void) {
    if (mMainLight) mRenderer->mSceneManager->destroyLight(BaseGame::mainLightName);
    mRenderer->mSceneManager->setSkyBoxEnabled(false);
    for (auto p : myPartyAlive) {
        delete p;
    }
    for (auto p : enemyPartyAlive) {
        delete p;
    }
}

void SingleplayerGame::initGUI(void)
{
    mHUD = new HUD(*(mRenderer->mSceneManager), *mGUI, myPartyAlive,
            enemyPartyAlive, myPartyWaiting);
}

bool SingleplayerGame::go(void)
{
    // Create the scene
    createScene();
    myPartyAlive = myParty;
    myPartyWaiting = myParty;
    enemyPartyAlive = enemyParty;

    initGUI();
    mHUD->registerListener(this);
    mHUD->registerListener(&mSoundController);

    mMusicTrack = Mix_LoadMUS("assets/audio/castlewall.wav");
    Mix_FadeInMusic(mMusicTrack, -1, 1000);

    // setup listeners
    mRenderer->addFrameListener(this);
    return true;
}

static bool characterDead(Player* p) { return p->isDead(); }

bool SingleplayerGame::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mGameOver || mShutDown) {
        return true;
    }

    if (playerTurn) {
        if (myPartyWaiting.size() == 0) {
            playerTurn = false;
            myPartyWaiting = myPartyAlive;
        }
    }
    else {
        if (activeEnemy < enemyPartyAlive.size()) {
            // placeholder enemy action
            enemyPartyAlive.at(activeEnemy)->physicalAttack(
                    *myPartyAlive.at(0/* place holder*/));
            ++activeEnemy;
        }
        else {
            // enemy turns over
            activeEnemy = 0;
            playerTurn = true;
            onRoundOver();
        }
    }
    for(auto player : myPartyAlive) {
        player->collision();
        player->checkTime();
    }

    mHUD->update();

    // remove all of the dead characters
    myPartyAlive.erase(std::remove_if(myPartyAlive.begin(), myPartyAlive.end(), characterDead),
            myPartyAlive.end());
    myPartyWaiting.erase(std::remove_if(myPartyWaiting.begin(), myPartyWaiting.end(),
                characterDead), myPartyWaiting.end());
    enemyPartyAlive.erase(std::remove_if(enemyPartyAlive.begin(), enemyPartyAlive.end(),
                characterDead), enemyPartyAlive.end());
    mHUD->refocusAfterCharacterDeath();

    if (myPartyAlive.empty() || enemyPartyAlive.empty()) {
        mGameOver = true;
        for(auto player: myParty) {
            player->stopEmittingAll();
        }
        for(auto enemy: enemyParty) {
            enemy->stopEmittingAll();
        }
        // throw up lose game gui
        std::cout << (enemyPartyAlive.empty() ? "You win" : "You lose") << std::endl;
        mHUD->alertGameOver(enemyPartyAlive.empty());
    }

    return true;
}

bool SingleplayerGame::keyPressed(const OIS::KeyEvent &arg) {
    if (mGameOver || playerTurn) {
        mHUD->injectKeyDown(arg);
    }
    return true;
}

void SingleplayerGame::onHUDPhysicalSelect(Player* attacker, Player* target) {
    attacker->lookAt(target);
    attacker->physicalAttack(*target);
    std::cout << target->info().name << ": " << target->info().health << std::endl;
}

void SingleplayerGame::onHUDSpecialSelect(Player* attacker, Player* target) {
    std::cout << "Special " << std::endl;
    attacker->lookAt(target);
    attacker->specialAttack(*target);
}

void SingleplayerGame::onHUDItemSelect(Player* user, Player* target) {
    std::cout << "Item " << std::endl;
    user->target = target;
    user->item(*target);
    // user->setVisible(ParticleType::Item, true);
    // user->setEmitting(ParticleType::Item, true);
}

void SingleplayerGame::onHUDGuardSelect(Player* user) {
    std::cout << "Guard " << std::endl;
    user->setVisible(ParticleType::Item, true);
    user->setEmitting(ParticleType::Guard, true);
}

void SingleplayerGame::onHUDPlayAgain() {
    // reset game state, i.e. iterate over each player reseting hp, sp, and then reset HUD
    for (Player* p : myParty) {
        p->reset();
    }
    myPartyAlive = myParty;
    myPartyWaiting = myParty;

    for (Player* p : enemyParty) {
        p->reset();
    }
    enemyPartyAlive = enemyParty;

    mGameOver = false;
    playerTurn = true;

    mHUD->update();
}

void SingleplayerGame::onHUDQuit() {
    mShutDown = true;
}

void SingleplayerGame::onRoundOver(void) {
    for(auto p : myParty) {
        p->setEmitting(ParticleType::Guard, false);
    }
}
