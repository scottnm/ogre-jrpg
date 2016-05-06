#include "GameManager.h"
#include "SingleplayerGame.h"

GameManager::GameManager()
    : mRenderer(RenderingEngine::getSingleton()),
      mGUI(new GUISystem()),
      mGame(nullptr),
      mShutDown(false)
       {
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

    // Create startup menu
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor()
        .setDefaultImage("WindowsLook/MouseArrow");

    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    
    mRoot = wmgr.loadLayoutFromFile("character_select.layout");
    auto charRoot = mRoot->getChild("FrameColourRect");
    mGUI->addAndSetWindowGroup("charSelect", mRoot);

    mRoot->getChild("ConfirmButton")->setEnabled(false);
    mRoot->getChild("ConfirmButton")->setText("Select 3 Fighters");
    mRoot->getChild("ConfirmButton")->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&GameManager::guiCbConfirmButton, this));

    possible_players.push_back(mPlayerBank.getPlayerInfo("Cannibal Corpse"));
    possible_players.push_back(mPlayerBank.getPlayerInfo("Spooky Boo"));
    possible_players.push_back(mPlayerBank.getPlayerInfo("Rattlebones"));
    possible_players.push_back(mPlayerBank.getPlayerInfo("Goldilocks"));
    possible_players.push_back(mPlayerBank.getPlayerInfo("Legs"));
    possible_players.push_back(mPlayerBank.getPlayerInfo("Jigbert"));
    possible_players.push_back(mPlayerBank.getPlayerInfo("DJ Du$ty"));
    possible_players.push_back(mPlayerBank.getPlayerInfo("Marshall"));
    possible_players.push_back(mPlayerBank.getPlayerInfo("Babe Ruth"));

    frames.push_back(charRoot->getChild("FrameColourRect0x0"));
    frames.push_back(charRoot->getChild("FrameColourRect0x1"));
    frames.push_back(charRoot->getChild("FrameColourRect0x2"));
    frames.push_back(charRoot->getChild("FrameColourRect1x0"));
    frames.push_back(charRoot->getChild("FrameColourRect1x1"));
    frames.push_back(charRoot->getChild("FrameColourRect1x2"));
    frames.push_back(charRoot->getChild("FrameColourRect2x0"));
    frames.push_back(charRoot->getChild("FrameColourRect2x1"));
    frames.push_back(charRoot->getChild("FrameColourRect2x2"));

    for (int i = 0; i < 9; i++) {
        frames[i]->getChild("Image_Portrait")->subscribeEvent(CEGUI::Window::EventMouseClick,
            CEGUI::Event::Subscriber(&GameManager::guiCbClickFrame, this));
        frames[i]->getChild("Image_Portrait")->setID(i);
    }
    
    for(int i = 0; i < 9; ++i) {
        const PlayerInfo& info = possible_players[i];
        CEGUI::Window* frame = frames[i]; 

        frame->getChild("Label_Name")->setText(info.name);
        frame->getChild("Image_Portrait")->setProperty("Image", info.img);
        std::string hpText = std::to_string(info.health) + "/" + std::to_string(info.healthMax);
        frame->getChild("Value_HP")->setText(hpText);
        std::string spText = std::to_string(info.specialPoints) + "/" + std::to_string(info.specialPointsMax);
        frame->getChild("Value_SP")->setText(spText);
        frame->getChild("Value_Damage")->setText(std::to_string(info.damage));
        frame->getChild("Value_Armor")->setText(std::to_string(info.armor));
        frame->getChild("Value_Accuracy")->setText(std::to_string((int)(info.accuracy * 100)) + "%");
    }

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
    return true;
}

bool GameManager::mouseMoved( const OIS::MouseEvent &arg ) {
    mGUI->mouseMoved(arg);
    return true;
}

bool GameManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
    mGUI->mousePressed(arg, id);
    return true;
}

bool GameManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {
    mGUI->mouseReleased(arg, id);
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
    mGUI->switchToWindowGroup("charSelect");
}

// STARTUP MENU CALLBACKS
bool GameManager::guiCbClickFrame(const CEGUI::EventArgs& e) {
    CEGUI::Window* w = static_cast<const CEGUI::WindowEventArgs&>(e).window;
    std::cout << w->getID() << std::endl;

    auto i = party.find(w->getID());
    if (i != party.end()) {
        party.erase(i);
        frames[w->getID()]->setProperty("Colour",
            "tl:00000000 tr:00000000 bl:00000000 br:00000000");
    }
    else if (party.size() < 3) {
        party.insert(w->getID());
        frames[w->getID()]->setProperty("Colour",
            "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
    }

    if (party.size() == 3) {
        mRoot->getChild("ConfirmButton")->setEnabled(true);
        mRoot->getChild("ConfirmButton")->setText("Fight!");
    }
    else {
        mRoot->getChild("ConfirmButton")->setEnabled(false);
        mRoot->getChild("ConfirmButton")->setText("Select 3 Fighters");
    }
    return true;
}

bool GameManager::guiCbConfirmButton(const CEGUI::EventArgs& e) {
    if (party.size() == 3) {
        if (mGame != nullptr) delete mGame;
        mGame = new SingleplayerGame(mRenderer, mGUI, &mPlayerBank, &mSoundBank);
        std::vector<std::string> playerNames;
        for(auto e : party) {
            playerNames.push_back(possible_players[e].name);
            frames[e]->setProperty("Colour",
                "tl:00000000 tr:00000000 bl:00000000 br:00000000");
        }
        mRoot->getChild("ConfirmButton")->setEnabled(false);
        mRoot->getChild("ConfirmButton")->setText("Select 3 Fighters");
        party.clear();

        mGame->go(playerNames);
        CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
        return true;
    }

    return true;
}

bool GameManager::guiCbQuit(const CEGUI::EventArgs& e) {
    mShutDown = true;
    return true;
}
