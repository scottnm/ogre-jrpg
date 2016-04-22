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
    if (mHUD) delete mHUD;
    mRenderer->mSceneManager->setSkyBoxEnabled(false);
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

    n = new Ninja(scnMgr, mRoomRoot, 0);
    n->setPosition(Ogre::Vector3(500, 0, 250));
    player_list.push_back(n);

    Player* p4 = new Player(scnMgr, mRoomRoot);
    p4->setPosition(Ogre::Vector3(-500, 50, 0));
    player_list.push_back(p4);

    n->lookAt(p4);
    // for (int i = 0; i < player_list.size(); ++i)
    //     player_list[i]->sceneNode->showBoundingBox(true);
    n->startGuardSystem(true);
    n->startFireSystem(true);
    n->startIceSystem(true);
    n->startFlareSystem(true);


    // Set Camera Position
    camera->setPosition(Ogre::Vector3(-1000, 250, -1000));
    camera->lookAt(Ogre::Vector3(0, 0, 0));

}

void SingleplayerGame::destroyScene(void) {
    if (mMainLight) mRenderer->mSceneManager->destroyLight(BaseGame::mainLightName);
}

void SingleplayerGame::initGUI(void)
{
    mHUD = new HUD(*mGUI, *this);
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
    GameObject* player = player_list[1];
    // player->sceneNode->showBoundingBox(true);
    if(n)
    {
        for (auto i = 0; i < n->particleSystemList.size(); ++i)
        {
            Ogre::ParticleSystem* ps = n->particleSystemList[i];
            if(ps->getEmitting() && i > 2)
            {
                int numParticles = ps->getNumParticles();
                for (int j = 0; j < numParticles; ++j)
                {
                    Ogre::Particle* p = ps->getParticle(j);
                    if(p)
                    {
                        Ogre::Vector3 pPos = p->position;
                        Ogre::SceneNode* sn = n->particleNodeList[j];
                        Ogre::Quaternion orientation = sn->_getDerivedOrientation();
                        Ogre::Vector3 possiblePos = orientation * pPos;
                        Ogre::Vector3 snPos = sn->_getDerivedPosition();
                        Ogre::Vector3 worldPos = Ogre::Vector3(snPos.x + possiblePos.x,
                                                               snPos.y + possiblePos.y,
                                                               snPos.z + possiblePos.z);
                        Ogre::AxisAlignedBox boundingBox = player->sceneNode->_getWorldAABB();
                        bool hit = boundingBox.intersects(worldPos);
                        if(hit)
                        {
                            // std::cout << "hit" << std::endl;
                            switch (i)
                            {
                                case 3:
                                    if(j == numParticles - 1)
                                        n->visibilityFireSystem(false);
                                    n->startFireSystem(false);
                                    // std::cout << "ajaja" << std::endl;
                                    break;
                                case 4:
                                    if(j == numParticles - 1)
                                        n->visibilityIceSystem(false);
                                    n->startIceSystem(false);
                                    // std::cout << "ajajajajaja" << std::endl;
                                    break;
                                case 5:
                                    if(j == numParticles - 1)
                                        n->visibilityFlareSystem(false);
                                    n->startFlareSystem(false);
                                    // std::cout << std::endl << std::endl << std::endl;
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool SingleplayerGame::keyPressed(const OIS::KeyEvent &arg) {
    mHUD->injectKeyDown(arg);
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
    mHUD->injectKeyUp(arg);
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

void SingleplayerGame::onHUDPhysicalSelect() {
    std::cout << "Attack" << std::endl;
}

void SingleplayerGame::onHUDSpecialSelect() {
    std::cout << "Special" << std::endl;
}

void SingleplayerGame::onHUDItemSelect() {
    std::cout << "Item" << std::endl;
}

void SingleplayerGame::onHUDGuardSelect() {
    std::cout << "Guard" << std::endl;
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
