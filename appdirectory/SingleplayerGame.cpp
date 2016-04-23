/*
-----------------------------------------------------------------------------
Filename:    SingleplayerGame.cpp
-----------------------------------------------------------------------------
 
This source file is adopted from MinimalOgre.cpp
http://www.ogre3d.org/tikiwiki/tiki-index.php?page=MinimalOgre-cpp
-----------------------------------------------------------------------------
*/

#include "SingleplayerGame.h"
#include <string>
#include <algorithm>

SingleplayerGame::SingleplayerGame(RenderingEngine* renderer, GUISystem* gui,
    PlayerBank* playerBank, SoundBank* soundBank)
    : BaseGame(renderer, gui, playerBank), mGameOver(false), mSoundController(soundBank),
      playerTurn(true), activeEnemy(0) {
}

SingleplayerGame::~SingleplayerGame(void) {
    destroyScene();
    if (mHUD) delete mHUD;
    mRenderer->mRoot->removeFrameListener(this);
}

void SingleplayerGame::createScene(void){
    auto scnMgr = mRenderer->mSceneManager;
    auto root = scnMgr->getRootSceneNode();
    Ogre::Camera* camera = mRenderer->mCamera;
    mRoomRoot = root->createChildSceneNode();
    scnMgr->setSkyBox(true, "Examples/MorningSkyBox", 5000, false);


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
            mPlayerBank->getPlayerInfo("Cannibal Corpse"));
    p->setPosition(Ogre::Vector3(500, 50, 200));
    myParty.push_back(p);

    Player* p2 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("Spooky Boo"));
    p2->setPosition(Ogre::Vector3(500, 50, 0));
    myParty.push_back(p2);

    Player* p3 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("Rattlebones"));
    p3->setPosition(Ogre::Vector3(500, 50, -200));
    myParty.push_back(p3);
    

    Player* p4 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("Mecha-Scoot"));
    p4->setPosition(Ogre::Vector3(-500, 50, 200));
    enemyParty.push_back(p4);

    Player* p5 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("SSJVirginia"));
    p5->setPosition(Ogre::Vector3(-500, 50, 0));
    enemyParty.push_back(p5);

    Player* p6 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("Metal Scoot"));
    p6->setPosition(Ogre::Vector3(-500, 50, -200));
    enemyParty.push_back(p6);

    // Set Camera Position
    camera->setPosition(Ogre::Vector3(-1000, 250, -1000));
    camera->lookAt(Ogre::Vector3(0, 0, 0));
}

void SingleplayerGame::destroyScene(void) {
    if (mMainLight) mRenderer->mSceneManager->destroyLight(BaseGame::mainLightName);
    mRenderer->mSceneManager->setSkyBoxEnabled(false);
    for (auto p : myParty) {
        delete p;
    }
    for (auto p : enemyParty) {
        delete p;
    }
}

void SingleplayerGame::initGUI(void)
{
    mHUD = new HUD(*(mRenderer->mSceneManager), *mGUI, myParty, enemyParty, myPartyWaiting);
}

bool SingleplayerGame::go(void)
{
    // Create the scene
    createScene();
    myPartyWaiting = myParty;
    initGUI();
    mHUD->registerListener(this);
    mHUD->registerListener(&mSoundController);

    // setup listeners
    mRenderer->addFrameListener(this);
    return true;
}

static bool characterDead(Player* p) { return p->isDead(); }

bool SingleplayerGame::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mShutDown || mGameOver)
    {
        return true;
    }

    if (playerTurn) {
        if (myPartyWaiting.size() == 0) {
            playerTurn = false;
            myPartyWaiting = myParty;
        }
    }
    else {
        if (activeEnemy < enemyParty.size()) {
            // enemy action
            enemyParty.at(activeEnemy)->physicalAttack(*myParty.at(0/* place holder*/));
            // queue next enemy
            ++activeEnemy;
        }
        else {
            // enemy turns over
            activeEnemy = 0;
            playerTurn = true;
        }
    }

    mHUD->update();

    // remove all of the dead characters
    myParty.erase(std::remove_if(myParty.begin(), myParty.end(), characterDead), myParty.end());
    myPartyWaiting.erase(std::remove_if(myPartyWaiting.begin(), myPartyWaiting.end(),
                characterDead), myPartyWaiting.end());
    enemyParty.erase(std::remove_if(enemyParty.begin(), enemyParty.end(), characterDead), enemyParty.end());
    mHUD->refocusAfterCharacterDeath();

    if (myParty.empty()) {
        mGameOver = true;
        // throw up lose game gui
        std::cout << "You lose" << std::endl;
    }

    return true;
}

bool SingleplayerGame::keyPressed(const OIS::KeyEvent &arg) {
    if (playerTurn && myPartyWaiting.size() > 0) {
        mHUD->injectKeyDown(arg);
    }
    return true;
}

void SingleplayerGame::onHUDPhysicalSelect(Player* attacker, Player* target) {
    attacker->physicalAttack(*target);
    std::cout << target->info.name << ": " << target->info.health << std::endl;
}

void SingleplayerGame::onHUDSpecialSelect(Player* attacker, Player* target) {
    std::cout << "Special " << std::endl;
}

void SingleplayerGame::onHUDItemSelect(Player* user, Player* target) {
    std::cout << "Item " << std::endl;
}

void SingleplayerGame::onHUDGuardSelect() {
    std::cout << "Guard " << std::endl;
}
