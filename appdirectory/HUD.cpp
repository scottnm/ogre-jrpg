#include "HUD.h"
#include <algorithm>

const Ogre::String HUD::windowName = "HUD";

HUD::HUD(GUISystem& gui)
    : mGUI(gui), mRoot(nullptr), mOptionSelected(3), mItemsMenuVisible(false) {

    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    mRoot = wmgr.loadLayoutFromFile("menu.layout");

    auto MenuFrame = mRoot->getChild("Menu_Frame");

    mOptionSelects[3] = MenuFrame->getChild("Physical_select");
    mOptionSelects[2] = MenuFrame->getChild("Special_select");
    mOptionSelects[1] = MenuFrame->getChild("Items_select");
    mOptionSelects[0] = MenuFrame->getChild("Guard_select");

    mGUI.addAndSetWindowGroup(HUD::windowName, mRoot);
}

void HUD::injectKeyDown(const OIS::KeyEvent& arg) {
    if (!mItemsMenuVisible) {
        int oldOption = mOptionSelected;
        switch(arg.key) {
            case OIS::KC_UP:
                mOptionSelected = std::min(3, mOptionSelected + 1);
                break;
            case OIS::KC_DOWN:
                mOptionSelected = std::max(0, mOptionSelected - 1);
                break;
            case OIS::KC_RETURN: {
                mRoot->getChild("Menu_Frame")->hide();
                auto itemFrame = mRoot->getChild("Item_Frame");
                itemFrame->show();
                itemFrame->activate();
                mItemsMenuVisible = true;
                break;
             }
            default:
                return;
        }
        mOptionSelects[oldOption]->hide();
        mOptionSelects[mOptionSelected]->show();
        mOptionSelects[mOptionSelected]->activate();
    }
}

void HUD::injectKeyUp(const OIS::KeyEvent& arg) {
    // pass
    (void) arg;
}
