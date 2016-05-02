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
    : BaseGame(renderer, gui, playerBank), mGameOver(false), mAnimationRunning(false),
      mSoundBank(soundBank), playerTurn(true), activeEnemy(0) {
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

bool SingleplayerGame::go(void)
{
    // Create the scene
    createScene();
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

    bool particleEmitting = false;

    for(auto player : myParty) {
        player->collision(mSoundBank);
    }

    for(auto enemy : enemyParty) {
        enemy->collision(mSoundBank);
    }

    for(auto c : myParty) {
        if(!particleEmitting) {
            particleEmitting = c->emittingParticles;
        }
        c->mAnimationController->updateAnimationTime(evt.timeSinceLastFrame);
    }

    for(auto c : enemyParty) {
        if(!particleEmitting) {
            particleEmitting = c->emittingParticles;
        }
        c->mAnimationController->updateAnimationTime(evt.timeSinceLastFrame);
    }

    if(mAnimationRunning || mGameOver || mShutDown || particleEmitting) {
        return true;
    }

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
                int randomAction = rand() % 3;
                int randomTarget = rand() % myPartyAlive.size();
                Player* target = myPartyAlive.at(randomTarget);
                switch(randomAction) {
                    case 0:
                        onHUDGuardSelect(enemyPartyAlive.at(activeEnemy));
                        break;
                    case 1:
                        onHUDPhysicalSelect(enemyPartyAlive.at(activeEnemy), target);
                        break;
                    case 2:
                        onHUDSpecialSelect(enemyPartyAlive.at(activeEnemy), target);
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

    for(auto player : myPartyAlive) {
        player->checkTime();
    }

    for(auto enemy : enemyPartyAlive) {
        enemy->checkTime();
    }

    for(auto player : myParty) {
        if(player->isDead()) {
            player->stopEmittingAll();
        }
    }

    for(auto enemy : enemyParty) {
        if(enemy->isDead()) {
            enemy->stopEmittingAll();
        }
    }

    mHUD->updatePartyInfo();
    clearDeadCharacters();

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
        if (enemyPartyAlive.empty()) {
            mHUD->alertGameOver(true);
            mSoundBank->play("game_win_fx");
        }
        else {
            mHUD->alertGameOver(false);
            mSoundBank->play("game_lose_fx");
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
    bool emittingParticle = false;
    for(auto player : myPartyAlive) {
        if(player->emittingParticles) {
            emittingParticle = player->emittingParticles;
        }
    }

    for(auto enemy : enemyPartyAlive) {
        if(enemy->emittingParticles) {
            emittingParticle = enemy->emittingParticles;
        }
    }
    if (!emittingParticle && !mAnimationRunning && (mGameOver || playerTurn)) {
        mHUD->injectKeyDown(arg);
    }
    return true;
}

void SingleplayerGame::onHUDPhysicalSelect(Player* attacker, Player* target) {
    mAnimationRunning = true;
    bool& animationRunning = mAnimationRunning;
    bool attackSuccessful = attacker->attemptPhysicalAttack();

    AnimationCallback cb = [&animationRunning, attacker, target,
                      attackSuccessful](void)-> void{
        if (attackSuccessful) {
            attacker->lookAt(target);
            attacker->physicalAttack(*target);
        }
        else {
            // miss logic
        }
        animationRunning = false;
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
}

void SingleplayerGame::onHUDSpecialSelect(Player* attacker, Player* target) {
    mAnimationRunning = true;
    bool& animationRunning = this->mAnimationRunning;
    AnimationCallback cb = [&animationRunning, attacker, target](void)-> void{
        attacker->lookAt(target);
        attacker->specialAttack(*target);
        animationRunning = false;
        attacker->mAnimationController->runIdleAnimation();
    };
    attacker->mAnimationController->runAnimation(AnimationType::Special, cb);
    mSoundBank->play("special_attack_fx"); 
}

void SingleplayerGame::onHUDItemSelect(Player* user, Player* target) {
    mAnimationRunning = true;
    bool& animationRunning = this->mAnimationRunning;
    Inventory& inventory = mInventory;
    AnimationCallback cb = [&animationRunning, user, target, &inventory](void)-> void{
        target->item();
        inventory.useItem(*target);
        animationRunning = false;
        user->mAnimationController->runIdleAnimation();
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
    mAnimationRunning = true;
    bool& animationRunning = this->mAnimationRunning;
    AnimationCallback cb = [&animationRunning, user](void)-> void{
        user->guard();
        animationRunning = false;
        user->mAnimationController->runIdleAnimation();
    };
    user->mAnimationController->runAnimation(AnimationType::Guard, cb);
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

    mHUD->updatePartyInfo();
    mInventory.reset();
}

void SingleplayerGame::onHUDQuit() {
    mShutDown = true;
}

void SingleplayerGame::partyReset(std::vector<Player*>& party) {
    for(auto p : party) {
        p->unguard();
    }
}
