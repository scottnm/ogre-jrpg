#include "GUISystem.h"

GUISystem::GUISystem() {
    mGUIRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile("SampleBrowser.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile("VanillaCommonDialogs.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile("GameMenu.scheme");
    


    mContext = &CEGUI::System::getSingleton().getDefaultGUIContext();
    mActiveWindow = nullptr;
    mRootWindow = CEGUI::WindowManager::getSingleton().createWindow(
            "DefaultWindow", "_MasterRoot");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(mRootWindow);
}

GUISystem::~GUISystem() {
    for(auto& windowItr : mWindowDatabase) {
        CEGUI::WindowManager::getSingleton().destroyWindow(windowItr.second);
    }
    CEGUI::OgreRenderer::destroySystem();
}

void GUISystem::addNewWindowGroup(std::string _n, CEGUI::Window* _r) {
    mWindowDatabase.emplace(_n, _r);
    _r->hide();
    mRootWindow->addChild(_r);
}

void GUISystem::switchToWindowGroup(std::string _n) {
    auto window = mWindowDatabase.find(_n)->second;
    if (mActiveWindow) mActiveWindow->hide();
    window->show();
    window->activate();
    mActiveWindow = window;
}

void GUISystem::addAndSetWindowGroup(std::string _n, CEGUI::Window* _r) {
    mWindowDatabase.emplace(_n, _r);
    if (mActiveWindow) mActiveWindow->hide();
    mRootWindow->addChild(_r);
    _r->show();
    _r->activate();
    mActiveWindow = _r;
}

CEGUI::Window* GUISystem::retrieveWindowGroup(std::string _n) {
    return mWindowDatabase.find(_n)->second;
}

void GUISystem::destroyWindowGroup(std::string _n) {
    auto windowItr = mWindowDatabase.find(_n);
    CEGUI::WindowManager::getSingleton().destroyWindow(windowItr->second);
    mWindowDatabase.erase(windowItr);
    if (windowItr->second == mActiveWindow) {
        mActiveWindow = nullptr;
    }
}

// Inputs

static CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID) {
    switch(buttonID) {
        case OIS::MB_Left:
            return CEGUI::LeftButton;

        case OIS::MB_Right:
            return CEGUI::RightButton;

        case OIS::MB_Middle:
            return CEGUI::MiddleButton;

        default:
            return CEGUI::LeftButton;
    }
}

bool GUISystem::keyPressed(const OIS::KeyEvent& arg) {
    mContext->injectKeyDown((CEGUI::Key::Scan)arg.key);
    mContext->injectChar((CEGUI::Key::Scan)arg.text);
    return true;
}

bool GUISystem::keyReleased(const OIS::KeyEvent& arg) {
    mContext->injectKeyUp((CEGUI::Key::Scan)arg.key);
    return true;
}

bool GUISystem::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
    mContext->injectMouseButtonDown(convertButton(id));
    return true;
}

bool GUISystem::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
    mContext->injectMouseButtonUp(convertButton(id));
    return true;
}

#define MAGIC_MSFT_MOUSEWHEEL 120.0f
bool GUISystem::mouseMoved(const OIS::MouseEvent& arg) {
    mContext->injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
    if (arg.state.Z.rel) {
        mContext->injectMouseWheelChange(arg.state.Z.rel / MAGIC_MSFT_MOUSEWHEEL);
    }
    return true;
}
