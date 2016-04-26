#include "HUD.h"
#include <algorithm>
#include <OgreStringConverter.h>

const Ogre::String HUD::windowName = "HUD";
// const Ogre::String HUD::items[4] = {"Apple", "Cool thing", "jimjam", "sweater"};

enum HUD_ID {
    PHYSICAL_ID = 0,
    SPECIAL_ID,
    ITEMS_ID,
    GUARD_ID
};

HUD::HUD(GUISystem& gui, HUDListener& listener,
         std::vector<Player*>& myParty, std::vector<Player*>& enemyParty, std::vector<std::pair<Item,int>> items) 
    : mGUI(gui), mListener(listener), mState(HUD_STATE::ACTION_MENU_ACTIVE),
      myParty(myParty), enemyParty(enemyParty), items(items) {

    mOptionSelected = 0;
    mItemsFocused = true;
    mItemsTotal = items.size();
    mItemSelected = 0;

    myPartyActiveTarget = 0;
    enemyPartyActiveTarget = 0;

    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    mRoot = wmgr.loadLayoutFromFile("scaling_menu.layout");
    auto targetWindow = wmgr.createWindow("TaharezLook/Button", "TargetingIcon");
    targetWindow->setText("Targeting");
    targetWindow->setSize(CEGUI::USize(CEGUI::UDim(0.10, 0), CEGUI::UDim(0.05, 0)));
    targetWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
    targetWindow->hide();
    mRoot->addChild(targetWindow);

    auto MenuFrame = mRoot->getChild("Menu_Frame");

    mOptionSelects[PHYSICAL_ID] = MenuFrame->getChild("Physical_select");
    mOptionSelects[SPECIAL_ID] = MenuFrame->getChild("Special_select");
    mOptionSelects[ITEMS_ID] = MenuFrame->getChild("Items_select");
    mOptionSelects[GUARD_ID] = MenuFrame->getChild("Guard_select");

    // prep gui with info

    mRoot->getChild("Item_Frame")->getChild("Items_StaticText")->
        setText(items[mItemSelected].first.name + " a (" + std::to_string(items[mItemSelected].second) + ")");

    auto p0Frame = mRoot->getChild("Party_Frame")->getChild("PartyMember0_Frame");
    auto p1Frame = mRoot->getChild("Party_Frame")->getChild("PartyMember1_Frame");
    auto p2Frame = mRoot->getChild("Party_Frame")->getChild("PartyMember2_Frame");
    CEGUI::Window* frames[3] = {p0Frame, p1Frame, p2Frame};
    for(Player* player : myParty) { 
        PlayerInfo& info = player->info;
        CEGUI::Window* frame = frames[player->id]; 
        frame->getChild("PM_Name_StaticText")->setText(info.name);
        frame->getChild("PM_HP_Left_StaticText")->setText(
                Ogre::StringConverter::toString(info.health));
        frame->getChild("PM_HP_Total_StaticText")->setText(
                Ogre::StringConverter::toString(info.healthMax));
        frame->getChild("PM_SP_Left_StaticText")->setText(
                Ogre::StringConverter::toString(info.specialPoints));
        frame->getChild("PM_SP_Total_StaticText")->setText(
                Ogre::StringConverter::toString(info.specialPointsMax));
    }

    charSelected = p0Frame;
    updateFocusedCharacter(0);
    mGUI.addAndSetWindowGroup(HUD::windowName, mRoot);
}

HUD::~HUD() {
    mGUI.destroyWindowGroup(HUD::windowName);
}

void HUD::injectKeyDown(const OIS::KeyEvent& arg) {
    if (mState == HUD_STATE::ACTION_MENU_ACTIVE) {
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
                    case SPECIAL_ID:
                        switchToTargetMenu();
                        break;
                    case ITEMS_ID:
                        switchToItemMenu();
                        break;
                    case GUARD_ID:
                        mListener.onHUDGuardSelect();
                        break;
                }
                break;
            case OIS::KC_TAB:
                mListener.onHUDCycleCharacter();
                break;
            default:
                return;
        }
        mOptionSelects[oldOption]->hide();
        mOptionSelects[mOptionSelected]->show();
        mOptionSelects[mOptionSelected]->activate();
    }
    else if (mState == HUD_STATE::ITEMS_MENU_ACTIVE) {
        if (mItemsFocused) {
            switch(arg.key) {
                case OIS::KC_LEFT:
                    //mItemSelected = mItemSelected == 0 ? mItemsTotal : mItemSelected - 1;
                    break;
                case OIS::KC_RIGHT:
                    //mItemSelected = (mItemSelected + 1) % mItemsTotal;
                    break;
                // case OIS::KC_DOWN: {
                //     mItemsFocused = false;
                //     auto itemFrame = mRoot->getChild("Item_Frame");
                //     itemFrame->getChild("Items_select")->hide();
                //     itemFrame->getChild("Back_select")->show();
                //     itemFrame->getChild("Back_select")->activate();
                //     break;
                // }

                case OIS::KC_UP:
                    mOptionSelected = std::max(0, mItemSelected - 1);
                    break;
                case OIS::KC_DOWN:
                    mOptionSelected = std::min((int)items.size() - 1, mItemSelected + 1);
                    break;

                case OIS::KC_RETURN:
                    switchToTargetMenu();
                    break;
                default:
                    return;
            }
            mRoot->getChild("Item_Frame")->getChild("Items_StaticText")->
        setText(items[mItemSelected].first.name + " a (" + std::to_string(items[mItemSelected].second) + ")");
        }
        else {
            switch(arg.key) {
                // case OIS::KC_RETURN: {
                //     switchToActionMenu();
                //     break;
                // }
                // case OIS::KC_UP: {
                //     mItemsFocused = true;
                //     auto itemFrame = mRoot->getChild("Item_Frame");
                //     itemFrame->getChild("Back_select")->hide();
                //     itemFrame->getChild("Items_select")->show();
                //     itemFrame->getChild("Items_select")->activate();
                //     break;
                // }
                default:
                    return;
            }
        }
    }
    else if (mState == HUD_STATE::TARGETING_MENU_ACTIVE) {
        switch(arg.key) {
            case OIS::KC_Q:
                enemyParty.at(enemyPartyActiveTarget)->hideTargetArrow();
                if (mPrevState == HUD_STATE::ACTION_MENU_ACTIVE) {
                    switchToActionMenu();
                }
                else {
                    switchToItemMenu();
                }
                break;
            case OIS::KC_RETURN:
                enemyParty.at(enemyPartyActiveTarget)->hideTargetArrow();
                switch(mOptionSelected) {
                    case PHYSICAL_ID:
                        mListener.onHUDPhysicalSelect();
                        break;
                    case SPECIAL_ID:
                        mListener.onHUDSpecialSelect();
                        break;
                    case ITEMS_ID:
                        mListener.onHUDItemSelect();
                        break;
                    default:
                        break;
                }
                switchToActionMenu(); 
                break;
            case OIS::KC_TAB:
                cycleTargetCharacter();
                break;
            default:
                break;
        }
    }
}

void HUD::switchToItemMenu(void) {
    mRoot->getChild("TargetingIcon")->hide();
    mPrevState = mState;
    mState = HUD_STATE::ITEMS_MENU_ACTIVE;
    mRoot->getChild("Menu_Frame")->hide();
    auto itemFrame = mRoot->getChild("Item_Frame");
    itemFrame->show();
    itemFrame->activate();
}

void HUD::switchToActionMenu(void) {
    mRoot->getChild("TargetingIcon")->hide();
    mPrevState = mState;
    mState = HUD_STATE::ACTION_MENU_ACTIVE;
    mItemsFocused = true;
    mRoot->getChild("Menu_Frame")->show();
    mRoot->getChild("Menu_Frame")->activate();
    auto itemFrame = mRoot->getChild("Item_Frame");
    itemFrame->hide();
    itemFrame->getChild("Items_select")->show();
    itemFrame->getChild("Items_select")->activate();
    itemFrame->getChild("Back_select")->hide();
}

void HUD::switchToTargetMenu(void) {
    mPrevState = mState;
    mState = HUD_STATE::TARGETING_MENU_ACTIVE;
    auto targetIcon = mRoot->getChild("TargetingIcon");
    targetIcon->show();
    targetIcon->activate();
    enemyParty.at(enemyPartyActiveTarget)->hideTargetArrow();
    enemyPartyActiveTarget = 0;
    enemyParty.at(enemyPartyActiveTarget)->showTargetArrow();
}

void HUD::updateFocusedCharacter(int charId) {
    charSelected->setProperty("Colour",
            "tl:0000000 tr:00000000 bl:00000000 br:00000000");

    static const CEGUI::String tmpStart = "PartyMember";
    static const CEGUI::String tmpEnd = "_Frame";
    Ogre::String id = Ogre::StringConverter::toString(charId);
    auto focusedChildFrame = mRoot->getChild("Party_Frame")->getChild(
            tmpStart + CEGUI::String(id.c_str()) + tmpEnd);

    focusedChildFrame->setProperty("Colour",
            "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
    charSelected = focusedChildFrame;
}

void HUD::injectKeyUp(const OIS::KeyEvent& arg) {
    // pass
    (void) arg;
}

void HUD::cycleTargetCharacter(void) {
    enemyParty.at(enemyPartyActiveTarget)->hideTargetArrow();
    enemyPartyActiveTarget = (enemyPartyActiveTarget + 1) % enemyParty.size();
    enemyParty.at(enemyPartyActiveTarget)->showTargetArrow();
}
