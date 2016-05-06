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
    : BaseGame(renderer, gui, playerBank), mGameOver(false), mAttackRunning(false),
      mSoundBank(soundBank), playerTurn(true), activeEnemy(0), mWaveCnt(1) {
}

SingleplayerGame::~SingleplayerGame(void) {
    Mix_FadeOutMusic(1000);
    Mix_FreeMusic(mMusicTrack);
    destroyScene();
    if (mHUD) delete mHUD;
    mRenderer->mRoot->removeFrameListener(this);
}

void SingleplayerGame::createScene(std::vector<std::string> partyNames){
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
            mPlayerBank->getPlayerInfo(partyNames[0]),
            Ogre::Vector3(500, 0, 200), mSoundBank);
    myParty.push_back(p);

    Player* p2 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo(partyNames[1]),
            Ogre::Vector3(500, 0, 0), mSoundBank);
    myParty.push_back(p2);

    Player* p3 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo(partyNames[2]),
            Ogre::Vector3(500, 0, -200), mSoundBank);
    myParty.push_back(p3);
    

    Player* p4 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("Mecha-Scoot"),
            Ogre::Vector3(-500, 0, 200), mSoundBank);
    enemyParty.push_back(p4);

    Player* p5 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("SSJVirginia"),
            Ogre::Vector3(-500, 0, 0), mSoundBank);
    enemyParty.push_back(p5);

    Player* p6 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("Metal Scoot"),
            Ogre::Vector3(-500, 0, -200), mSoundBank);
    enemyParty.push_back(p6);

    // Set Camera Position
    cameraInitialPosition = Ogre::Vector3(1100, 250, 700);
    cameraInitialLookAt = Ogre::Vector3(0, 0, 0);
    camera->setPosition(cameraInitialPosition);
    camera->lookAt(cameraInitialLookAt);
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
    mHUD = new HUD(*(mRenderer->mSceneManager), *mGUI, this, myPartyAlive,
            enemyPartyAlive, myPartyWaiting, mInventory);
}

bool SingleplayerGame::go(std::vector<std::string> pn)
{
    // Create the scene
    createScene(pn);
    myPartyAlive = myParty;
    myPartyWaiting = myParty;
    enemyPartyAlive = enemyParty;

    initGUI();

    mMusicTrack = Mix_LoadMUS("assets/audio/castlewall.wav");
    Mix_FadeInMusic(mMusicTrack, -1, 1000);

    // setup listeners
    mRenderer->addFrameListener(this);
    return true;
}

bool SingleplayerGame::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

    for(auto c : myParty) {
        c->mAnimationController->updateAnimationTime(evt.timeSinceLastFrame);
        c->mParticleController->updateParticles();
        c->mDamageIndicatorController->injectTime(evt.timeSinceLastFrame);
    }

    for(auto c : enemyParty) {
        c->mAnimationController->updateAnimationTime(evt.timeSinceLastFrame);
        c->mParticleController->updateParticles();
        c->mDamageIndicatorController->injectTime(evt.timeSinceLastFrame);
    }

    if(mAttackRunning || mGameOver || mShutDown) {
        return true;
    }

    mHUD->update();
    clearDeadCharacters();

    if (myPartyAlive.empty() || enemyPartyAlive.empty()) {

        mGameOver = true;

        // throw up lose game gui
        mHUD->alertGameOver(enemyPartyAlive.empty());
        if (enemyPartyAlive.empty()) {
            mHUD->alertGameOver(true);
            mSoundBank->play("game_win_fx");
        }
        else {
            mHUD->alertGameOver(false);
            mSoundBank->play("game_lose_fx");
        }
        return true;
    }

    mRenderer->mCamera->setPosition(cameraInitialPosition);
    mRenderer->mCamera->lookAt(cameraInitialLookAt);

    if (playerTurn) {
        if (myPartyWaiting.size() == 0) {
            playerTurn = false;
            myPartyWaiting = myPartyAlive;
            partyReset(enemyParty);
        }
    }
    else {
        if (activeEnemy < enemyPartyAlive.size()) {
            if(!enemyPartyAlive.at(activeEnemy)->isDead()) {
                // placeholder enemy action
                int actionOptions = 0;
                if(enemyPartyAlive.at(activeEnemy)->info().specialPoints > 0) {
                    actionOptions = 3;
                }
                else {
                    actionOptions = 2;
                }
                int randomAction = rand() % actionOptions;
                int randomTarget = rand() % myPartyAlive.size();
                Player* target = myPartyAlive.at(randomTarget);
                while(target->isDead()) {
                    randomTarget = rand() % myPartyAlive.size();
                    target = myPartyAlive.at(randomTarget);
                }
                switch(randomAction) {
                    case 0:
                        onHUDGuardSelect(enemyPartyAlive.at(activeEnemy));
                        break;
                    case 1:
                        onHUDPhysicalSelect(enemyPartyAlive.at(activeEnemy), target);
                        break;
                    case 2:
                        if (target->info().specialPoints > 0) {
                            onHUDSpecialSelect(enemyPartyAlive.at(activeEnemy), target);
                        }
                        else {
                            if (rand() % 2 == 0) {
                                onHUDGuardSelect(enemyPartyAlive.at(activeEnemy));
                            }
                            else {
                                onHUDPhysicalSelect(enemyPartyAlive.at(activeEnemy), target);
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
            ++activeEnemy;
        }
        else {
            // enemy turns over
            activeEnemy = 0;
            playerTurn = true;
            partyReset(myParty);
        }
    }


    return true;
}

void SingleplayerGame::clearDeadCharacters(void) {
    // remove all of the dead characters
    for(auto citr = myPartyAlive.begin(); citr != myPartyAlive.end(); ++citr) {
        Player* p = *citr;
        if (p->isDead()) {
            AnimationCallback cb = [](void)-> void {};
            p->mAnimationController->runAnimation(AnimationType::Death, cb);
            p->mParticleController->stopEmittingAll();
            mSoundBank->play("death_fx");
            mSoundBank->play("grunt_fx");
            citr = myPartyAlive.erase(citr);
            if (citr == myPartyAlive.end()) {
                break;
            }
        }
    }

    for(auto citr = enemyPartyAlive.begin(); citr != enemyPartyAlive.end(); ++citr) {
        Player* p = *citr;
        if (p->isDead()) {
            AnimationCallback cb = [](void)-> void {};
            p->mAnimationController->runAnimation(AnimationType::Death, cb);
            mSoundBank->play("death_fx");
            mSoundBank->play("grunt_fx");

            citr = enemyPartyAlive.erase(citr);
            if (citr == enemyPartyAlive.end()) {
                break;
            }
        }
    }

    myPartyWaiting.erase(
            std::remove_if(myPartyWaiting.begin(), myPartyWaiting.end(),
                [](Player* p)->bool{ return p->isDead(); }),
            myPartyWaiting.end());

    mHUD->refocusAfterCharacterDeath();
}

bool SingleplayerGame::keyPressed(const OIS::KeyEvent &arg) {
    if (!mAttackRunning && (mGameOver || playerTurn)) {
        mHUD->injectKeyDown(arg);
    }
    return true;
}

void SingleplayerGame::onHUDPhysicalSelect(Player* attacker, Player* target) {
    mAttackRunning = true;
    bool attackSuccessful = attacker->attemptPhysicalAttack();

    attacker->lookAt(target);
    AnimationCallback cb = [this, attacker, target, attackSuccessful](void)-> void{
        if (attackSuccessful) {
            attacker->lookAt(target);
            attacker->physicalAttack(*target);
            if (!target->isDead()) {
                time_t physicalStartTime = time(nullptr);
                ParticleEndCheckCallback endCheck = [physicalStartTime](void) -> bool {
                    return difftime(time(nullptr), physicalStartTime) >= 1;
                };
                ParticleCallback onEnd = [](void) -> void {};
                target->mParticleController->runParticleSystem(PT_Physical, endCheck, onEnd);
            }
        }
        else {
            attacker->missAttack(*target);
        }
        mAttackRunning = false;
        attacker->mAnimationController->runIdleAnimation();
    };

    attacker->mAnimationController->runAnimation(AnimationType::Physical, cb);
    if (attackSuccessful) {
        mSoundBank->play("physical_woosh_fx");
        mSoundBank->play("physical_impact_fx");
    }
    else {
        mSoundBank->play("physical_miss_fx");
    }
    Ogre::Vector3 camPos = (attacker->sceneNode->_getDerivedPosition() + Ogre::Vector3(0, 200, 0)) - (target->sceneNode->_getDerivedPosition() + Ogre::Vector3(0, 100, 0));
    camPos.normalise();
    camPos *= 300;
    camPos += (attacker->sceneNode->_getDerivedPosition() + Ogre::Vector3(0, 200, 0));
    mRenderer->mCamera->setPosition(camPos);
    mRenderer->mCamera->lookAt(target->sceneNode->_getDerivedPosition() + Ogre::Vector3(0, 100, 0));
}

void SingleplayerGame::onHUDSpecialSelect(Player* attacker, Player* target) {

    mAttackRunning = true;
    attacker->lookAt(target);

    // prep animation 
    AnimationCallback cb = [attacker, target](void)-> void{
        attacker->mAnimationController->runIdleAnimation();
    };
    attacker->mAnimationController->runAnimation(AnimationType::Special, cb);

    // prep particles
    int randNum = rand() % 3;
    std::string sfx;
    ParticleType pt;
    ParticleEndCheckCallback endCheck;
    if (randNum == 0) {
        endCheck = [attacker, target](void) -> bool {
            return attacker->mParticleController->checkFireCollision(target->sceneNode);
        };
        sfx = "fireball_attack_fx";
        pt = PT_Fire;
    }
    else if (randNum == 1) {
        endCheck = [attacker, target](void) -> bool {
            return attacker->mParticleController->checkIceCollision(target->sceneNode);
        };
        sfx = "ice_attack_fx";
        pt = PT_Ice;
    }
    else {
        endCheck = [attacker, target](void) -> bool {
            return attacker->mParticleController->checkFlareCollision(target->sceneNode);
        };
        sfx = "flare_attack_fx";
        pt = PT_Flare;
    }

    ParticleCallback onEnd = [this, attacker, target, sfx](void) -> void {
        mAttackRunning = false;
        attacker->specialAttack(*target);
    };
    attacker->mParticleController->runParticleSystem(pt, endCheck, onEnd);

    mSoundBank->play("special_attack_fx");

    Ogre::Vector3 camPos = (attacker->sceneNode->_getDerivedPosition() + Ogre::Vector3(0, 200, 0)) - (target->sceneNode->_getDerivedPosition() + Ogre::Vector3(0, 100, 0));
    camPos.normalise();
    camPos *= 300;
    camPos += (attacker->sceneNode->_getDerivedPosition() + Ogre::Vector3(0, 200, 0));
    mRenderer->mCamera->setPosition(camPos);
    mRenderer->mCamera->lookAt(target->sceneNode->_getDerivedPosition() + Ogre::Vector3(0, 100, 0));
}

void SingleplayerGame::onHUDItemSelect(Player* user, Player* target) {
    mAttackRunning = true;
    bool& attackRunning = this->mAttackRunning;
    Inventory& inventory = mInventory;
    AnimationCallback cb = [&attackRunning, user, target, &inventory](void)-> void{
        target->item();
        inventory.useItem(*target);
        attackRunning = false;
        user->mAnimationController->runIdleAnimation();

        time_t itemStartTime = time(nullptr);
        ParticleEndCheckCallback endCheck = [itemStartTime](void) -> bool {
            return difftime(time(nullptr), itemStartTime) >= 1;
        };
        ParticleCallback onEnd = [](void) -> void {};
        target->mParticleController->runParticleSystem(PT_Item, endCheck, onEnd);
    };
    user->mAnimationController->runAnimation(AnimationType::Item, cb);
    if (mInventory.getCurrentItem().isOffensive) {
        mSoundBank->play("item_off_fx");
    }
    else {
        mSoundBank->play("item_def_fx");
    }
}

void SingleplayerGame::onHUDGuardSelect(Player* user) {
    mAttackRunning = true;
    AnimationCallback cb = [this, user](void)-> void{
        mAttackRunning = false;
        user->mAnimationController->runIdleAnimation();
    };
    user->mAnimationController->runAnimation(AnimationType::Guard, cb);
    user->guard();

    mSoundBank->play("guard_fx");
}

void SingleplayerGame::onHUDCycleCharacter(void) {
    mSoundBank->play("hud_cycle_fx");
}

void SingleplayerGame::onHUDOptionSelect(void) {
    mSoundBank->play("option_select_fx");
}

void SingleplayerGame::onHUDNavigation(void) {
    mSoundBank->play("option_nav_fx");
}

void SingleplayerGame::onHUDPlayAgain() {
    // reset game state, i.e. iterate over each player reseting hp, sp, and then reset HUD
    for (Player* p : myParty) {
        p->reset();
        p->mAnimationController->runIdleAnimation();
    }
    myPartyAlive = myParty;
    myPartyWaiting = myParty;

    for (Player* p : enemyParty) {
        p->reset();
        p->mAnimationController->runIdleAnimation();
    }
    enemyPartyAlive = enemyParty;

    mGameOver = false;
    playerTurn = true;

    mHUD->update();
    mInventory.reset();

    mWaveCnt = 1;
    mHUD->updateWaveCounter(mWaveCnt);
}

void SingleplayerGame::onHUDPlayNext() {
    myPartyWaiting = myPartyAlive;
    mHUD->reset();

    for (Player* p : enemyParty) {
        p->reset();
        p->mAnimationController->runIdleAnimation();
    }
    enemyPartyAlive = enemyParty;

    mGameOver = false;
    playerTurn = true;

    mHUD->update();
    mHUD->updateWaveCounter(++mWaveCnt);
}

void SingleplayerGame::onHUDQuit() {
    mShutDown = true;
}

void SingleplayerGame::partyReset(std::vector<Player*>& party) {
    for(auto p : party) {
        p->unguard();
    }
}
