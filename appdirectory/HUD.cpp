#include "HUD.h"
#include <algorithm>
#include <OgreStringConverter.h>

const Ogre::String HUD::windowName = "HUD";
const Ogre::String HUD::mockItems[4] = {"Apple", "Cool thing", "jimjam", "sweater"};

enum HUD_ID {
    PHYSICAL_ID = 0,
    SPECIAL_ID,
    ITEMS_ID,
    GUARD_ID
};

HUD::HUD(GUISystem& gui, HUDListener& listener,
        const PlayerInfo& p1, const PlayerInfo& p2, const PlayerInfo& p3)
    : mGUI(gui), mListener(listener) {

    mOptionSelected = 0;
    mItemsMenuVisible = false;
    mItemsFocused = true;
    mItemsTotal = 4;
    mItemSelected = 0;


    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    mRoot = wmgr.loadLayoutFromFile("scaling_menu.layout");

    auto MenuFrame = mRoot->getChild("Menu_Frame");

    mOptionSelects[PHYSICAL_ID] = MenuFrame->getChild("Physical_select");
    mOptionSelects[SPECIAL_ID] = MenuFrame->getChild("Special_select");
    mOptionSelects[ITEMS_ID] = MenuFrame->getChild("Items_select");
    mOptionSelects[GUARD_ID] = MenuFrame->getChild("Guard_select");

    // prep gui with info

    mRoot->getChild("Item_Frame")->getChild("Items_StaticText")->
        setText(mockItems[mItemSelected]);

    auto p1Frame = mRoot->getChild("Party_Frame")->getChild("PartyMember1_Frame");
    auto p2Frame = mRoot->getChild("Party_Frame")->getChild("PartyMember2_Frame");
    auto p3Frame = mRoot->getChild("Party_Frame")->getChild("PartyMember3_Frame");
    CEGUI::Window* frames[3] = {p1Frame, p2Frame, p3Frame};
    PlayerInfo players[3] = {p1, p2, p3};
    for(int i = 0; i < 3; ++i) {
        auto frame = frames[i];
        auto player = players[i];
        frame->getChild("PM_Name_StaticText")->setText(player.name);
        frame->getChild("PM_HP_Left_StaticText")->setText(
                Ogre::StringConverter::toString(player.health));
        frame->getChild("PM_HP_Total_StaticText")->setText(
                Ogre::StringConverter::toString(player.healthMax));
        frame->getChild("PM_SP_Left_StaticText")->setText(
                Ogre::StringConverter::toString(player.specialPoints));
        frame->getChild("PM_SP_Total_StaticText")->setText(
                Ogre::StringConverter::toString(player.specialPointsMax));
    }

    mGUI.addAndSetWindowGroup(HUD::windowName, mRoot);
}

HUD::~HUD() {
    mGUI.destroyWindowGroup(HUD::windowName);
}

void HUD::injectKeyDown(const OIS::KeyEvent& arg) {
    if (!mItemsMenuVisible) {
        int oldOption = mOptionSelected;
        switch(arg.key) {
            case OIS::KC_UP:
                mOptionSelected = std::max(0, mOptionSelected - 1);
                break;
            case OIS::KC_DOWN:
                mOptionSelected = std::min(3, mOptionSelected + 1);
                break;
            case OIS::KC_RETURN:
                switch (mOptionSelected) {
                    case PHYSICAL_ID:
                        mListener.onHUDPhysicalSelect();
                        break;
                    case SPECIAL_ID:
                        mListener.onHUDSpecialSelect();
                        break;
                    case ITEMS_ID:
                        switchToItemMenu();
                        break;
                    case GUARD_ID:
                        mListener.onHUDGuardSelect();
                        break;
                }
            default:
                return;
        }
        mOptionSelects[oldOption]->hide();
        mOptionSelects[mOptionSelected]->show();
        mOptionSelects[mOptionSelected]->activate();
    }
    else {
        if (mItemsFocused) {
            switch(arg.key) {
                case OIS::KC_LEFT:
                    mItemSelected = mItemSelected == 0 ? mItemsTotal : mItemSelected - 1;
                    break;
                case OIS::KC_RIGHT:
                    mItemSelected = (mItemSelected + 1) % mItemsTotal;
                    break;
                case OIS::KC_DOWN: {
                    mItemsFocused = false;
                    auto itemFrame = mRoot->getChild("Item_Frame");
                    itemFrame->getChild("Items_select")->hide();
                    itemFrame->getChild("Back_select")->show();
                    itemFrame->getChild("Back_select")->activate();
                    break;
                }
                case OIS::KC_RETURN:
                    mListener.onHUDItemSelect();
                    switchToActionMenu(); 
                    break;
                default:
                    return;
            }
            mRoot->getChild("Item_Frame")->getChild("Items_StaticText")->
                setText(mockItems[mItemSelected]);
        }
        else {
            switch(arg.key) {
                case OIS::KC_RETURN: {
                    switchToActionMenu();
                    break;
                }
                case OIS::KC_UP: {
                    mItemsFocused = true;
                    auto itemFrame = mRoot->getChild("Item_Frame");
                    itemFrame->getChild("Back_select")->hide();
                    itemFrame->getChild("Items_select")->show();
                    itemFrame->getChild("Items_select")->activate();
                    break;
                }
                default:
                    return;
            }
        }
    }
}

void HUD::switchToItemMenu(void) {
    mItemsMenuVisible = true;
    mRoot->getChild("Menu_Frame")->hide();
    auto itemFrame = mRoot->getChild("Item_Frame");
    itemFrame->show();
    itemFrame->activate();
}

void HUD::switchToActionMenu(void) {
    mItemsMenuVisible = false;
    mItemsFocused = true;
    mRoot->getChild("Menu_Frame")->show();
    mRoot->getChild("Menu_Frame")->activate();
    auto itemFrame = mRoot->getChild("Item_Frame");
    itemFrame->hide();
    itemFrame->getChild("Items_select")->show();
    itemFrame->getChild("Items_select")->activate();
    itemFrame->getChild("Back_select")->hide();
}

void HUD::injectKeyUp(const OIS::KeyEvent& arg) {
    // pass
    (void) arg;
}
