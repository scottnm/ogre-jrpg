#include "HUD.h"
#include <algorithm>

const Ogre::String HUD::windowName = "HUDRoot";
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


    mActionOptionFocused = 0;
    mItemsFocused = true;
    mItemsTotal = 4;
    mItemSelected = 0;

    myPartyFocused = 0;
    enemyPartyActiveTarget = 0;

    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    auto root = wmgr.createWindow("DefaultWindow", "HUDRoot");
    mMenuRoot = wmgr.loadLayoutFromFile("scaling_menu.layout");
    root->addChild(mMenuRoot);
    mEndStateRoot = wmgr.loadLayoutFromFile("end_state.layout");
    root->addChild(mEndStateRoot);
    mEndStateRoot->hide();
    auto targetWindow = wmgr.createWindow("TaharezLook/Button", "TargetingIcon");
    targetWindow->setText("Targeting");
    targetWindow->setSize(CEGUI::USize(CEGUI::UDim(0.10, 0), CEGUI::UDim(0.05, 0)));
    targetWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
    targetWindow->hide();
    mMenuRoot->addChild(targetWindow);

    auto MenuFrame = mMenuRoot->getChild("Menu_Frame");

    mActionOptions[PHYSICAL_ID] = MenuFrame->getChild("Physical_select");
    mActionOptions[SPECIAL_ID] = MenuFrame->getChild("Special_select");
    mActionOptions[ITEMS_ID] = MenuFrame->getChild("Items_select");
    mActionOptions[GUARD_ID] = MenuFrame->getChild("Guard_select");

    mMenuRoot->getChild("Item_Frame")->getChild("Items_StaticText")->
        setText(mockItems[mItemSelected]);

    auto p0Frame = mMenuRoot->getChild("Party_Frame")->getChild("PartyMember0_Frame");
    auto p1Frame = mMenuRoot->getChild("Party_Frame")->getChild("PartyMember1_Frame");
    auto p2Frame = mMenuRoot->getChild("Party_Frame")->getChild("PartyMember2_Frame");
    CEGUI::Window* frames[3] = {p0Frame, p1Frame, p2Frame};

    for(int i = 0; i < 3; ++i) {
        Player* character = myParty[i];
        CEGUI::Window* frame = frames[i]; 
        characterInfoWindows.emplace(character, frame);

        const PlayerInfo& info = character->info();
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
    mGUI.addAndSetWindowGroup(HUD::windowName, root);
}

HUD::~HUD() {
    mGUI.destroyWindowGroup(HUD::windowName);
}

void HUD::injectKeyDown(const OIS::KeyEvent& arg) {
    if (mState == HUD_STATE::ACTION_MENU_ACTIVE) {
        int oldOption = mActionOptionFocused;
        switch(arg.key) {
            case OIS::KC_UP:
                notifyHUDNavigation();
                mActionOptionFocused = std::max(0, mActionOptionFocused - 1);
                break;
            case OIS::KC_DOWN:
                notifyHUDNavigation();
                mActionOptionFocused = std::min(3, mActionOptionFocused + 1);
                break;
            case OIS::KC_RETURN:
                switch (mActionOptionFocused) {
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
                        mActionOptions[mActionOptionFocused]->hide();
                        mActionOptionFocused = 0;
                        mActionOptions[0]->show();
                        mActionOptions[0]->activate();
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
        mActionOptions[oldOption]->hide();
        mActionOptions[mActionOptionFocused]->show();
        mActionOptions[mActionOptionFocused]->activate();
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
                    auto itemFrame = mMenuRoot->getChild("Item_Frame");
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
            mMenuRoot->getChild("Item_Frame")->getChild("Items_StaticText")->
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
                    auto itemFrame = mMenuRoot->getChild("Item_Frame");
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
                switch(mActionOptionFocused) {
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
                dequeueActiveCharacter();
                setTargetArrowVisible(enemyParty.at(enemyPartyActiveTarget), false);
                mActionOptions[mActionOptionFocused]->hide();
                mActionOptionFocused = 0;
                mActionOptions[0]->show();
                mActionOptions[0]->activate();
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
    else if (mState == HUD_STATE::GAME_OVER) {
        if (arg.key == OIS::KC_RETURN) {
            mEndStateRoot->hide();
            if (mPlayAgainOptionFocused) {
                notifyPlayAgain();
                switchToActionMenu();
            }
            else {
                notifyQuit();
                switchToActionMenu();
            }
        }
        else if (arg.key == OIS::KC_UP || arg.key == OIS::KC_DOWN) {
            mPlayAgainOptionFocused = !mPlayAgainOptionFocused;
            mEndStateRoot->getChild("PlayAgain_select")->setVisible(mPlayAgainOptionFocused);
            mEndStateRoot->getChild("Quit_select")->setVisible(!mPlayAgainOptionFocused);
        }
    }
}

void HUD::switchToItemMenu(void) {
    mMenuRoot->getChild("TargetingIcon")->hide();
    mPrevState = mState;
    mState = HUD_STATE::ITEMS_MENU_ACTIVE;
    mMenuRoot->getChild("Menu_Frame")->hide();
    auto itemFrame = mMenuRoot->getChild("Item_Frame");
    itemFrame->show();
    itemFrame->activate();
}

void HUD::switchToActionMenu(void) {
    mMenuRoot->getChild("TargetingIcon")->hide();
    mPrevState = mState;
    mState = HUD_STATE::ACTION_MENU_ACTIVE;
    mItemsFocused = true;
    mMenuRoot->getChild("Menu_Frame")->show();
    mMenuRoot->getChild("Menu_Frame")->activate();
    auto itemFrame = mMenuRoot->getChild("Item_Frame");
    itemFrame->hide();
    itemFrame->getChild("Items_select")->show();
    itemFrame->getChild("Items_select")->activate();
    itemFrame->getChild("Back_select")->hide();
}

void HUD::switchToTargetMenu(void) {
    mPrevState = mState;
    mState = HUD_STATE::TARGETING_MENU_ACTIVE;
    auto targetIcon = mMenuRoot->getChild("TargetingIcon");
    targetIcon->show();
    targetIcon->activate();
    setTargetArrowVisible(enemyParty.at(enemyPartyActiveTarget), false);
    enemyPartyActiveTarget = 0;
    setTargetArrowVisible(enemyParty.at(enemyPartyActiveTarget), true);
}

void HUD::refocusAfterCharacterDeath(void) {
    if (myParty.size() == 0) {
        charSelected->setProperty("Colour",
            "tl:0000000 tr:00000000 bl:00000000 br:00000000");
    }
    else {
        myPartyFocused = std::min(myPartyFocused, myPartyWaiting.size());
        updateFocusedCharacter(myPartyWaiting.at(myPartyFocused));
    }
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
    myPartyFocused = (myPartyFocused + 1) % myPartyWaiting.size();
    updateFocusedCharacter(myPartyWaiting.at(myPartyFocused));
}

void HUD::dequeueActiveCharacter(void) {
    myPartyWaiting.erase(myPartyWaiting.begin() + myPartyFocused);

    if (myPartyWaiting.size() <= myPartyFocused) {
        myPartyFocused = 0;
    }

    if (myPartyWaiting.size() > 0) {
        updateFocusedCharacter(myPartyWaiting.at(myPartyFocused));
    }
    else {
        // have to use the myParty queue since the myParty waiting queue is empty
        updateFocusedCharacter(myParty.at(0));
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

void HUD::update(void) {
    for(auto character : myParty) {
        auto infoWindow = characterInfoWindows.find(character)->second;
        const PlayerInfo& info = character->info();
        infoWindow->getChild("PM_HP_Left_StaticText")->setText(
                Ogre::StringConverter::toString(info.health));
        infoWindow->getChild("PM_SP_Left_StaticText")->setText(
                Ogre::StringConverter::toString(info.specialPoints));
    }
}

void HUD::alertGameOver(bool userWins) {
    mPrevState = mState;
    mState = HUD_STATE::GAME_OVER;
    mPlayAgainOptionFocused = true;
    mEndStateRoot->show();
    mEndStateRoot->activate();
    mEndStateRoot->getChild("Win_label")->setVisible(userWins);
    mEndStateRoot->getChild("Lose_label")->setVisible(!userWins);
}

void HUD::notifyPhysicalSelect(void) {
    Player* attacker = myPartyWaiting.at(myPartyFocused);
    Player* target = enemyParty.at(enemyPartyActiveTarget);
    for(auto hl : mListeners) {
        hl->onHUDPhysicalSelect(attacker, target);
    }
}

void HUD::notifySpecialSelect(void) {
    Player* attacker = myPartyWaiting.at(myPartyFocused);
    Player* target = enemyParty.at(enemyPartyActiveTarget);
    for(auto hl : mListeners) {
        hl->onHUDSpecialSelect(attacker, target);
    }
}

void HUD::notifyItemSelect(void) {
    Player* user = myPartyWaiting.at(myPartyFocused);
    Player* target = enemyParty.at(enemyPartyActiveTarget);
    for(auto hl : mListeners) {
        hl->onHUDItemSelect(user, target);
    }
}

void HUD::notifyGuardSelect(void) {
    for(auto hl : mListeners) {
        hl->onHUDGuardSelect();
    }
}

void HUD::notifyCharacterCycle(void) {
    for(auto hl : mListeners) {
        hl->onHUDCycleCharacter();
    }
}

void HUD::notifyHUDOptionSelect(void) {
    for(auto hl : mListeners) {
        hl->onHUDOptionSelect();
    }
}

void HUD::notifyHUDNavigation(void) {
    for(auto hl : mListeners) {
        hl->onHUDNavigation();
    }
}

void HUD::notifyPlayAgain(void) {
    for(auto hl : mListeners) {
        hl->onHUDPlayAgain();
    }
}

void HUD::notifyQuit(void) {
    for(auto hl : mListeners) {
        hl->onHUDQuit();
    }
}
