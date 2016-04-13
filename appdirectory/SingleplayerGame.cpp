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
        SoundBank* soundBank)
    : BaseGame(renderer, gui, soundBank) {
}

SingleplayerGame::~SingleplayerGame(void) {
    destroyScene();
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
    Player* p = new Player(scnMgr, mRoomRoot);
    p->setPosition(Ogre::Vector3(500, 50, 0));
    player_list.push_back(p);

    Player* p2 = new Player(scnMgr, mRoomRoot);
    p2->setPosition(Ogre::Vector3(500, 50, 200));
    player_list.push_back(p2);

    Player* p3 = new Player(scnMgr, mRoomRoot);
    p3->setPosition(Ogre::Vector3(500, 50, -200));
    player_list.push_back(p3);

    Player* p4 = new Player(scnMgr, mRoomRoot);
    p4->setPosition(Ogre::Vector3(-500, 50, 0));

    // Set Camera Position
    camera->setPosition(Ogre::Vector3(-1000, 250, -1000));
    camera->lookAt(Ogre::Vector3(0, 0, 0));

}

void SingleplayerGame::destroyScene(void) {
    if (mMainLight) mRenderer->mSceneManager->destroyLight(BaseGame::mainLightName);
}

void SingleplayerGame::initGUI(void)
{
    BaseGame::initGUI();
}

bool SingleplayerGame::go(void)
{
    // Create the scene
    initGUI();
    createScene();

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
    return true;
}

bool SingleplayerGame::keyPressed(const OIS::KeyEvent &arg) {
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
