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

HUD::HUD(GUISystem& gui, std::vector<Player*>& myParty, std::vector<Player*>& enemyParty,
         std::vector<Player*>& myPartyWaiting) 
    : mGUI(gui), mState(HUD_STATE::ACTION_MENU_ACTIVE),
      myParty(myParty), myPartyWaiting(myPartyWaiting), enemyParty(enemyParty) {

    mOptionSelected = 0;
    mItemsFocused = true;
    mItemsTotal = 4;
    mItemSelected = 0;

    myPartyFocused = 0;
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
        setText(mockItems[mItemSelected]);

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
    myPartyWaiting.at(0)->showTargetArrow();
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
                notifyHUDNavigation();
                mOptionSelected = std::max(0, mOptionSelected - 1);
                break;
            case OIS::KC_DOWN:
                notifyHUDNavigation();
                mOptionSelected = std::min(3, mOptionSelected + 1);
                break;
            case OIS::KC_RETURN:
                switch (mOptionSelected) {
                    case PHYSICAL_ID:
                    case SPECIAL_ID:
                        notifyHUDOptionSelect();
                        switchToTargetMenu();
                        break;
                    case ITEMS_ID:
                        notifyHUDOptionSelect();
                        switchToItemMenu();
                        break;
                    case GUARD_ID:
                        notifyGuardSelect();
                        dequeueActiveCharacter();
                        break;
                }
                break;
            case OIS::KC_TAB:
                notifyCharacterCycle();
                cycleActiveCharacter();
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
                    notifyHUDNavigation();
                    mItemSelected = mItemSelected == 0 ? mItemsTotal : mItemSelected - 1;
                    break;
                case OIS::KC_RIGHT:
                    notifyHUDNavigation();
                    mItemSelected = (mItemSelected + 1) % mItemsTotal;
                    break;
                case OIS::KC_DOWN: {
                    notifyHUDNavigation();
                    mItemsFocused = false;
                    auto itemFrame = mRoot->getChild("Item_Frame");
                    itemFrame->getChild("Items_select")->hide();
                    itemFrame->getChild("Back_select")->show();
                    itemFrame->getChild("Back_select")->activate();
                    break;
                }
                case OIS::KC_RETURN:
                    notifyHUDOptionSelect(); 
                    switchToTargetMenu();
                    break;
                default:
                    return;
            }
            mRoot->getChild("Item_Frame")->getChild("Items_StaticText")->
                setText(mockItems[mItemSelected]);
        }
        else {
            switch(arg.key) {
                case OIS::KC_RETURN:
                    notifyHUDOptionSelect();
                    switchToActionMenu();
                    break;
                case OIS::KC_UP: {
                    notifyHUDNavigation();
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
    else if (mState == HUD_STATE::TARGETING_MENU_ACTIVE) {
        switch(arg.key) {
            case OIS::KC_Q:
                notifyHUDOptionSelect();
                enemyParty.at(enemyPartyActiveTarget)->hideTargetArrow();
                if (mPrevState == HUD_STATE::ACTION_MENU_ACTIVE) {
                    switchToActionMenu();
                }
                else {
                    switchToItemMenu();
                }
                break;
            case OIS::KC_RETURN:
                dequeueActiveCharacter();
                enemyParty.at(enemyPartyActiveTarget)->hideTargetArrow();
                switch(mOptionSelected) {
                    case PHYSICAL_ID:
                        notifyPhysicalSelect();
                        break;
                    case SPECIAL_ID:
                        notifySpecialSelect();
                        break;
                    case ITEMS_ID:
                        notifyItemSelect();
                        break;
                    default:
                        break;
                }
                switchToActionMenu(); 
                break;
            case OIS::KC_TAB:
                notifyCharacterCycle();
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

void HUD::cycleActiveCharacter(void) {
    myPartyWaiting.at(myPartyFocused)->hideTargetArrow();
    myPartyFocused = (myPartyFocused + 1) % myPartyWaiting.size();
    myPartyWaiting.at(myPartyFocused)->showTargetArrow();
    updateFocusedCharacter(myPartyWaiting.at(myPartyFocused)->id);
}

void HUD::dequeueActiveCharacter(void) {
    myPartyWaiting.at(myPartyFocused)->hideTargetArrow();
    myPartyWaiting.erase(myPartyWaiting.begin() + myPartyFocused);

    if (myPartyWaiting.size() <= myPartyFocused) {
        myPartyFocused = 0;
    }

    if (myPartyWaiting.size() > 0) {
        updateFocusedCharacter(myPartyWaiting.at(myPartyFocused)->id);
        myPartyWaiting.at(myPartyFocused)->showTargetArrow();
    }
    else {
        // have to use the myParty queue since the myParty waiting queue is empty
        updateFocusedCharacter(myParty.at(0)->id);
        myParty.at(0)->showTargetArrow();
    }
}

void HUD::cycleTargetCharacter(void) {
    enemyParty.at(enemyPartyActiveTarget)->hideTargetArrow();
    enemyPartyActiveTarget = (enemyPartyActiveTarget + 1) % enemyParty.size();
    enemyParty.at(enemyPartyActiveTarget)->showTargetArrow();
}

void HUD::registerListener(HUDListener* hl) {
    mListeners.emplace(hl);
}

void HUD::notifyPhysicalSelect(void){
    for(auto hl : mListeners) {
        hl->onHUDPhysicalSelect();
    }
}

void HUD::notifySpecialSelect(void){
    for(auto hl : mListeners) {
        hl->onHUDSpecialSelect();
    }
}

void HUD::notifyItemSelect(void){
    for(auto hl : mListeners) {
        hl->onHUDItemSelect();
    }
}

void HUD::notifyGuardSelect(void){
    for(auto hl : mListeners) {
        hl->onHUDGuardSelect();
    }
}

void HUD::notifyCharacterCycle(void){
    for(auto hl : mListeners) {
        hl->onHUDCycleCharacter();
    }
}

void HUD::notifyHUDOptionSelect(void){
    for(auto hl : mListeners) {
        hl->onHUDOptionSelect();
    }
}

void HUD::notifyHUDNavigation(void){
    for(auto hl : mListeners) {
        hl->onHUDNavigation();
    }
}

