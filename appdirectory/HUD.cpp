#include "HUD.h"

const Ogre::String HUD::windowName = "HUD";

HUD::HUD(GUISystem& gui)
    : mGUI(gui), mRoot(nullptr), mOptionSelected(0), mItemsMenuVisible(false) {
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    mRoot = wmgr.loadLayoutFromFile("menu.layout");
    mGUI.addAndSetWindowGroup(HUD::windowName, mRoot);
}
