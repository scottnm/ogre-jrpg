#include "GameManager.h"
#include "SingleplayerGame.h"

GameManager::GameManager()
    : mRenderer(RenderingEngine::getSingleton()),
      mGUI(new GUISystem()),
      mGame(nullptr),
      mShutDown(false) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();

    mSoundBank.loadAudioDatabase("assets/audio/AudioSources.xml");
    mPlayerBank.loadPlayerDatabase("assets/characters/characters.xml");
}

GameManager::~GameManager(void) {
    if (mGame) delete mGame;

    //Remove ourself as a Window listener
    mRenderer->removeWindowEventListener(this);

    // unlatch our input devices from the game upon close
    if( mInputManager ) {
        mInputManager->destroyInputObject( mMouse );
        mInputManager->destroyInputObject( mKeyboard ); 
        OIS::InputManager::destroyInputSystem(mInputManager);
        mInputManager = 0;
    }
    delete mGUI;
    RenderingEngine::destroySingleton();
}

bool GameManager::runGame(void) {
    // If the mRenderer failed to initialize for
    // some reason then do not start the game
    if (mRenderer == nullptr) {
        return false;
    }

    //
    // Create startup menu
    //
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor()
        .setDefaultImage("WindowsLook/MouseArrow");

    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* StartupMenuGUI = wmgr.createWindow("DefaultWindow", "StartMenuGUI/Win");
    CEGUI::Window* spLauncher = wmgr.createWindow("WindowsLook/Button",
            "StartMenuGUI/spLauncherButton");
    spLauncher->setText("Launch");
    spLauncher->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
    spLauncher->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.5, 0)));
    StartupMenuGUI->addChild(spLauncher);
    spLauncher->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&GameManager::guiCbInitSinglePlayer, this));
    CEGUI::Window* quit = wmgr.createWindow("WindowsLook/Button", "StartMenuGUI/QuitButton");
    quit->setText("Quit");
    quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
    StartupMenuGUI->addChild(quit);
    quit->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&GameManager::guiCbQuit, this));
    mGUI->addAndSetWindowGroup("StartMenuGUI", StartupMenuGUI);

    //create FrameListener
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
 
    mRenderer->mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
    // setup OIS inputs
    mInputManager = OIS::InputManager::createInputSystem( pl );
 
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
 
    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
 
    //Set initial mouse clipping size
    windowResized(mRenderer->mWindow);
 
    //Register as a Window listener
    mRenderer->addWindowEventListener(this);
 
    mInputContext.mKeyboard = mKeyboard;
    mInputContext.mMouse = mMouse;

    mRenderer->addFrameListener(this);
    mRenderer->startRendering();

    return true;
}

bool GameManager::frameRenderingQueued(const Ogre::FrameEvent& evt) {
    if (mShutDown || mRenderer->isWindowClosed()) {
        return false;
    }
    
    if (mGame && mGame->isShutDown()) {
        closeGame();
    }
    
    mKeyboard->capture();
    mMouse->capture();

    return true;
}

bool GameManager::keyPressed( const OIS::KeyEvent &arg ) {
    if (arg.key == OIS::KC_ESCAPE) {
        if (mGame != nullptr) {
            closeGame();
        } else {
            mShutDown = true;
        }
    }
    mGUI->keyPressed(arg);
    if (mGame && !mGame->isShutDown()) { mGame->keyPressed(arg); }
    return true;
}

bool GameManager::keyReleased( const OIS::KeyEvent &arg ) {
    mGUI->keyReleased(arg);
    if (mGame && !mGame->isShutDown()) { mGame->keyReleased(arg); }
    return true;
}

bool GameManager::mouseMoved( const OIS::MouseEvent &arg ) {
    mGUI->mouseMoved(arg);
    if (mGame && !mGame->isShutDown()) { mGame->mouseMoved(arg); }
    return true;
}

bool GameManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
    mGUI->mousePressed(arg, id);
    if (mGame && !mGame->isShutDown()) { mGame->mousePressed(arg, id); }
    return true;
}

bool GameManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
    mGUI->mouseReleased(arg, id);
    if (mGame && !mGame->isShutDown()) { mGame->mouseReleased(arg, id); }
    return true;
}

void GameManager::windowResized(Ogre::RenderWindow* rw) {
    unsigned int width, height, depth;
    int left, top;
    mRenderer->getRenderMetrics(width, height, depth, left, top);
 
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

void GameManager::windowClosed(Ogre::RenderWindow* rw) {
    //Only close for window that created OIS (the main window in these demos)
    if (mRenderer->isRenderWindow(rw)) {
        if( mInputManager ) {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );
 
            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

void GameManager::closeGame(void) {
    delete mGame;
    mGame = nullptr;
    mGUI->switchToWindowGroup("StartMenuGUI");
}

//
// STARTUP MENU CALLBACKS
// 

bool GameManager::guiCbInitSinglePlayer(const CEGUI::EventArgs& e) {
    if (mGame != nullptr) delete mGame;
    mGame = new SingleplayerGame(mRenderer, mGUI, &mSoundBank);
    mGame->go();
    return true;
}

bool GameManager::guiCbQuit(const CEGUI::EventArgs& e) {
    mShutDown = true;
    return true;
}
