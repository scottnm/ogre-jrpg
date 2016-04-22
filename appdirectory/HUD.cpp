#include "HUD.h"
#include <algorithm>

const Ogre::String HUD::windowName = "HUD";
const Ogre::String HUD::mockItems[4] = {"Apple", "Cool thing", "jimjam", "sweater"};

enum HUD_ID {
    PHYSICAL_ID = 0,
    SPECIAL_ID,
    ITEMS_ID,
    GUARD_ID
};

HUD::HUD(Ogre::SceneManager& scnMgr, GUISystem& gui, std::vector<Player*>& myParty, std::vector<Player*>& enemyParty,
         std::vector<Player*>& myPartyWaiting) 
    : mGUI(gui), mState(HUD_STATE::ACTION_MENU_ACTIVE),
      myParty(myParty), myPartyWaiting(myPartyWaiting), enemyParty(enemyParty) {


    mOptionSelected = 0;
    mItemsFocused = true;
    mItemsTotal = 4;
    mItemSelected = 0;

    myPartyFocused = 0;
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

    mRoot->getChild("Item_Frame")->getChild("Items_StaticText")->
        setText(mockItems[mItemSelected]);

    auto p0Frame = mRoot->getChild("Party_Frame")->getChild("PartyMember0_Frame");
    auto p1Frame = mRoot->getChild("Party_Frame")->getChild("PartyMember1_Frame");
    auto p2Frame = mRoot->getChild("Party_Frame")->getChild("PartyMember2_Frame");
    CEGUI::Window* frames[3] = {p0Frame, p1Frame, p2Frame};

    for(int i = 0; i < 3; ++i) {
        Player* character = myParty[i];
        CEGUI::Window* frame = frames[i]; 
        characterInfoWindows.emplace(character, frame);

        PlayerInfo& info = character->info;
        frame->getChild("PM_Name_StaticText")->setText(info.name);
        frame->getChild("PM_Pic_StaticImage")->setProperty("Image", info.img);
        frame->getChild("PM_HP_Left_StaticText")->setText(
                Ogre::StringConverter::toString(info.health));
        frame->getChild("PM_HP_Total_StaticText")->setText(
                Ogre::StringConverter::toString(info.healthMax));
        frame->getChild("PM_SP_Left_StaticText")->setText(
                Ogre::StringConverter::toString(info.specialPoints));
        frame->getChild("PM_SP_Total_StaticText")->setText(
                Ogre::StringConverter::toString(info.specialPointsMax));

        auto targetBillboardSet = scnMgr.createBillboardSet();
        targetBillboardSet->setMaterialName("pixeltarget");
        targetBillboardSet->createBillboard(Ogre::Vector3::ZERO);

        auto targetSceneNode = character->sceneNode->
            createChildSceneNode(Ogre::Vector3(0, 200, 0));
        targetSceneNode->attachObject(targetBillboardSet);
        targetSceneNode->getAttachedObject(0)->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAX);
        targetSceneNode->setVisible(false);

        characterTargetArrows.emplace(character, targetSceneNode);
    }

    for (Player* e : enemyParty) {
        auto targetBillboardSet = scnMgr.createBillboardSet();
        targetBillboardSet->setMaterialName("pixeltarget");
        targetBillboardSet->createBillboard(Ogre::Vector3::ZERO);
        auto targetSceneNode = e->sceneNode->
            createChildSceneNode(Ogre::Vector3(0, 200, 0));
        targetSceneNode->attachObject(targetBillboardSet);
        targetSceneNode->getAttachedObject(0)->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAX);
        targetSceneNode->setVisible(false);
        characterTargetArrows.emplace(e, targetSceneNode);
    }


    charSelected = p0Frame;
    updateFocusedCharacter(myParty.at(0));
    setTargetArrowVisible(myParty.at(0), true);
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
                        mOptionSelects[mOptionSelected]->hide();
                        mOptionSelected = 0;
                        mOptionSelects[0]->show();
                        mOptionSelects[0]->activate();
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
                setTargetArrowVisible(enemyParty.at(enemyPartyActiveTarget), false);
                if (mPrevState == HUD_STATE::ACTION_MENU_ACTIVE) {
                    switchToActionMenu();
                }
                else {
                    switchToItemMenu();
                }
                break;
            case OIS::KC_RETURN:
                dequeueActiveCharacter();
                setTargetArrowVisible(enemyParty.at(enemyPartyActiveTarget), false);
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
                mOptionSelects[mOptionSelected]->hide();
                mOptionSelected = 0;
                mOptionSelects[0]->show();
                mOptionSelects[0]->activate();
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
    setTargetArrowVisible(enemyParty.at(enemyPartyActiveTarget), false);
    enemyPartyActiveTarget = 0;
    setTargetArrowVisible(enemyParty.at(enemyPartyActiveTarget), true);
}

void HUD::updateFocusedCharacter(Player* character) {
    charSelected->setProperty("Colour",
            "tl:0000000 tr:00000000 bl:00000000 br:00000000");
    auto focusedChildFrame = characterInfoWindows.find(character)->second;
    focusedChildFrame->setProperty("Colour",
            "tl:FFFF0000 tr:FFFF0000 bl:FFFF0000 br:FFFF0000");
    charSelected = focusedChildFrame;
}

void HUD::setTargetArrowVisible(Player* character, bool visible) {
    characterTargetArrows.find(character)->second->setVisible(visible);
}

void HUD::cycleActiveCharacter(void) {
    setTargetArrowVisible(myPartyWaiting.at(myPartyFocused), false);
    myPartyFocused = (myPartyFocused + 1) % myPartyWaiting.size();
    setTargetArrowVisible(myPartyWaiting.at(myPartyFocused), true);
    updateFocusedCharacter(myPartyWaiting.at(myPartyFocused));
}

void HUD::dequeueActiveCharacter(void) {
    setTargetArrowVisible(myPartyWaiting.at(myPartyFocused), false);
    myPartyWaiting.erase(myPartyWaiting.begin() + myPartyFocused);

    if (myPartyWaiting.size() <= myPartyFocused) {
        myPartyFocused = 0;
    }

    if (myPartyWaiting.size() > 0) {
        updateFocusedCharacter(myPartyWaiting.at(myPartyFocused));
        setTargetArrowVisible(myPartyWaiting.at(myPartyFocused), true);
    }
    else {
        // have to use the myParty queue since the myParty waiting queue is empty
        updateFocusedCharacter(myParty.at(0));
        setTargetArrowVisible(myParty.at(0), true);
    }
}

void HUD::cycleTargetCharacter(void) {
    setTargetArrowVisible(enemyParty.at(enemyPartyActiveTarget), false);
    enemyPartyActiveTarget = (enemyPartyActiveTarget + 1) % enemyParty.size();
    setTargetArrowVisible(enemyParty.at(enemyPartyActiveTarget), true);
}

void HUD::registerListener(HUDListener* hl) {
    mListeners.emplace(hl);
}

void HUD::frameUpdate(void) {
    for(auto character : myParty) {
        auto infoWindow = characterInfoWindows.find(character)->second;
        PlayerInfo& info = character->info;
        infoWindow->getChild("PM_HP_Left_StaticText")->setText(
                Ogre::StringConverter::toString(info.health));
        infoWindow->getChild("PM_SP_Left_StaticText")->setText(
                Ogre::StringConverter::toString(info.specialPoints));
    }
}

void HUD::notifyPhysicalSelect(void){
    Player* attacker = myPartyWaiting.at(myPartyFocused);
    Player* target = enemyParty.at(enemyPartyActiveTarget);
    for(auto hl : mListeners) {
        hl->onHUDPhysicalSelect(attacker, target);
    }
}

void HUD::notifySpecialSelect(void){
    Player* attacker = myPartyWaiting.at(myPartyFocused);
    Player* target = enemyParty.at(enemyPartyActiveTarget);
    for(auto hl : mListeners) {
        hl->onHUDSpecialSelect(attacker, target);
    }
}

void HUD::notifyItemSelect(void){
    Player* user = myPartyWaiting.at(myPartyFocused);
    Player* target = enemyParty.at(enemyPartyActiveTarget);
    for(auto hl : mListeners) {
        hl->onHUDItemSelect(user, target);
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

