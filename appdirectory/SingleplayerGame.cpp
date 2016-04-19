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

SingleplayerGame::SingleplayerGame(RenderingEngine* renderer, GUISystem* gui,
        SoundBank* soundBank, PlayerBank* playerBank)
    : BaseGame(renderer, gui, soundBank, playerBank) {
    focusedCharacterId = 0;
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
            mPlayerBank->getPlayerInfo("Chester"));
    p->setPosition(Ogre::Vector3(500, 50, 0));
    players.push_back(p);

    Player* p2 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("Scoot"));
    p2->setPosition(Ogre::Vector3(500, 50, 200));
    players.push_back(p2);

    Player* p3 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("Sygmund"));
    p3->setPosition(Ogre::Vector3(500, 50, -200));
    players.push_back(p3);
    

    Player* p4 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("Mecha-Scoot"));
    p4->setPosition(Ogre::Vector3(-500, 50, 0));
    enemies.push_back(p4);

    Player* p5 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("SSJVirginia"));
    p5->setPosition(Ogre::Vector3(-500, 50, 200));
    enemies.push_back(p5);

    Player* p6 = new Player(scnMgr, mRoomRoot,
            mPlayerBank->getPlayerInfo("SSJVirginia"));
    p6->setPosition(Ogre::Vector3(-500, 50, -200));
    enemies.push_back(p6);

    // Set Camera Position
    camera->setPosition(Ogre::Vector3(-1000, 250, -1000));
    camera->lookAt(Ogre::Vector3(0, 0, 0));
}

void SingleplayerGame::destroyScene(void) {
    if (mMainLight) mRenderer->mSceneManager->destroyLight(BaseGame::mainLightName);
    mRenderer->mSceneManager->setSkyBoxEnabled(false);
    for (auto p : players) {
        delete p;
    }
    for (auto p : enemies) {
        delete p;
    }
}

void SingleplayerGame::initGUI(void)
{
    mHUD = new HUD(*mGUI, *this, players);
}

bool SingleplayerGame::go(void)
{
    // Create the scene
    createScene();
    playersWaiting = players;
    initGUI();

    // setup listeners
    mRenderer->addFrameListener(this);
    return true;
}

bool SingleplayerGame::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mShutDown)
    {
        return true;
    }

    if (playersWaiting.size() == 0) {
        playersWaiting = players;
        std::cout << "Enemy turn" << std::endl;
    }
    return true;
}

bool SingleplayerGame::keyPressed(const OIS::KeyEvent &arg) {
    if (playersWaiting.size() > 0) {
        mHUD->injectKeyDown(arg);
    }
    switch(arg.key) {
        case OIS::KC_1:
            mSoundBank->play("1");
            break;
        case OIS::KC_2:
            mSoundBank->play("2");
            break;
        case OIS::KC_3:
            mSoundBank->play("3");
            break;
        default:
            break;
    }
    return true;
}
bool SingleplayerGame::keyReleased(const OIS::KeyEvent &arg) {
    if (playersWaiting.size() > 0) {
        mHUD->injectKeyUp(arg);
    }
    return true;
}
bool SingleplayerGame::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    return true;
}
bool SingleplayerGame::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    return true;
}
bool SingleplayerGame::mouseMoved(const OIS::MouseEvent &arg) {
    return true;
}

void SingleplayerGame::onHUDCycleCharacter() {
    focusedCharacterId = (focusedCharacterId + 1) % playersWaiting.size();
    mHUD->updateFocusedCharacter(playersWaiting.at(focusedCharacterId)->id);
}

void SingleplayerGame::dequeueActiveCharacter() {
    playersWaiting.erase(playersWaiting.begin() + focusedCharacterId);
    if (playersWaiting.size() <= focusedCharacterId) {
        focusedCharacterId = 0;
    }

    if (playersWaiting.size() > 0) {
        mHUD->updateFocusedCharacter(playersWaiting.at(focusedCharacterId)->id);
    }
    else {
        mHUD->updateFocusedCharacter(0);
    }
}

void SingleplayerGame::onHUDPhysicalSelect() {
    std::cout << "Attack " << std::endl;
    dequeueActiveCharacter();
}

void SingleplayerGame::onHUDSpecialSelect() {
    std::cout << "Special " << std::endl;
    dequeueActiveCharacter();
}

void SingleplayerGame::onHUDItemSelect() {
    std::cout << "Item " << std::endl;
    dequeueActiveCharacter();
}

void SingleplayerGame::onHUDGuardSelect() {
    std::cout << "Guard " << std::endl;
    dequeueActiveCharacter();
}

/*
bool SingleplayerGame::guiCbStart(const CEGUI::EventArgs& e)
{
    guiSys->switchToWindowGroup("SPDisplay");
    return true;
}

bool SingleplayerGame::guiCbResume(const CEGUI::EventArgs& e)
{
    mGamePaused = false;
    guiSys->switchToWindowGroup("SPDisplay");
    return true;
}

bool SingleplayerGame::guiCbQuit(const CEGUI::EventArgs& e)
{
    mShutDown = true;
    return true;
}
*/
