#include "HUD.h"
#include <algorithm>

using std::to_string;

const Ogre::String HUD::windowName = "HUDRoot";

enum HUD_ID {
    PHYSICAL_ID = 0,
    SPECIAL_ID,
    ITEMS_ID,
    GUARD_ID
};

HUD::HUD(Ogre::SceneManager& scnMgr,
         GUISystem& gui,
         HUDListener* listener,
         std::vector<Player*>& myParty,
         std::vector<Player*>& enemyParty,
         std::vector<Player*>& myPartyWaiting,
         Inventory& inventory) 
         : mGUI(gui),
           mListener(listener),
           mState(HUD_STATE::ACTION_MENU_ACTIVE),
           myParty(myParty),
           myPartyWaiting(myPartyWaiting),
           enemyParty(enemyParty),
           mInventory(inventory) {

    mActionOptionFocused = 0;
    mItemsFocused = true;

    myPartyFocused = 0;
    activeTarget = 0;

    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    //auto root = wmgr.createWindow("DefaultWindow", "HUDRoot");
    auto root = wmgr.loadLayoutFromFile("menu.layout");
    mMenuRoot = root->getChild("Control_Frame");
    mItemRoot = root->getChild("ItemInfo_Frame");
    mItemRoot->hide();
    mEndStateRoot = wmgr.loadLayoutFromFile("end_state.layout");
    root->addChild(mEndStateRoot);
    mEndStateRoot->hide();

    // targeting icon
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

    mMenuRoot->getChild("Item_Frame")->getChild("Items_StaticText")->setText(
            mInventory.getCurrentItemName());

    auto p0Frame = mMenuRoot->getChild("Party_Frame")->getChild("PartyMember0_Frame");
    auto p1Frame = mMenuRoot->getChild("Party_Frame")->getChild("PartyMember1_Frame");
    auto p2Frame = mMenuRoot->getChild("Party_Frame")->getChild("PartyMember2_Frame");
    CEGUI::Window* frames[3] = {p0Frame, p1Frame, p2Frame};

    for(int i = 0; i < 3; ++i) {
        Player* character = myParty[i];
        CEGUI::Window* frame = frames[i]; 
        characterInfoWindows.emplace(character, frame);

        const PlayerInfo& info = character->info();
        frame->getChild("PM_Name_Value")->setText(info.name);
        frame->getChild("PM_Pic")->setProperty("Image", info.img);
        std::string hpText = to_string(info.health) + "/" + to_string(info.healthMax);
        frame->getChild("PM_HP_Value")->setText(hpText);
        std::string spText = to_string(info.specialPoints) + "/" + to_string(info.specialPointsMax);
        frame->getChild("PM_SP_Value")->setText(spText);
        frame->getChild("PM_Damage_Value")->setText(to_string(info.damage));
        frame->getChild("PM_Armor_Value")->setText(to_string(info.armor));
        frame->getChild("PM_Accuracy_Value")->setText(to_string((int)(info.accuracy * 100)) + "%");

        auto targetBillboardSet = scnMgr.createBillboardSet();
        targetBillboardSet->setMaterialName("pixeltarget");
        targetBillboardSet->createBillboard(Ogre::Vector3::ZERO);

        auto targetSceneNode = character->sceneNode->
            createChildSceneNode(Ogre::Vector3(0, character->getHeight(), 0));
        targetSceneNode->attachObject(targetBillboardSet);
        targetSceneNode->getAttachedObject(0)->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAX);
        targetSceneNode->setVisible(false);

        characterTargetArrows.emplace(character, targetSceneNode);

        auto healthBarBillboardSet = scnMgr.createBillboardSet();
        healthBarBillboardSet->setMaterialName("healthbar");
        healthBarBillboardSet->createBillboard(Ogre::Vector3::ZERO);
        healthBarBillboardSet->setDefaultDimensions(10, 10);

        auto healthBarSceneNode = character->sceneNode->
            createChildSceneNode(Ogre::Vector3(0, character->getHeight() * 0.8, 0));
        healthBarSceneNode->attachObject(healthBarBillboardSet);
        healthBarSceneNode->getAttachedObject(0)->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAX);

        characterHealthBars.emplace(character, healthBarBillboardSet);

    }

    for (Player* e : enemyParty) {
        auto targetBillboardSet = scnMgr.createBillboardSet();
        targetBillboardSet->setMaterialName("pixeltarget");
        targetBillboardSet->createBillboard(Ogre::Vector3::ZERO);
        auto targetSceneNode = e->sceneNode->
            createChildSceneNode(Ogre::Vector3(0, e->getHeight(), 0));
        targetSceneNode->attachObject(targetBillboardSet);
        targetSceneNode->getAttachedObject(0)->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAX);
        targetSceneNode->setVisible(false);
        characterTargetArrows.emplace(e, targetSceneNode);

        auto healthBarBillboardSet = scnMgr.createBillboardSet();
        healthBarBillboardSet->setMaterialName("healthbar");
        healthBarBillboardSet->createBillboard(Ogre::Vector3::ZERO);
        healthBarBillboardSet->setDefaultDimensions(10, 10);

        auto healthBarSceneNode = e->sceneNode->
            createChildSceneNode(Ogre::Vector3(0, e->getHeight() * 0.8, 0));
        healthBarSceneNode->attachObject(healthBarBillboardSet);
        healthBarSceneNode->getAttachedObject(0)->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAX);

        characterHealthBars.emplace(e, healthBarBillboardSet);
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
                if (mInventory.items.size() == 0 && mActionOptionFocused == ITEMS_ID) {
                    mActionOptionFocused = SPECIAL_ID;
                }
                if (myPartyWaiting.at(myPartyFocused)->info().specialPoints == 0 &&
                        mActionOptionFocused == SPECIAL_ID) {
                    mActionOptionFocused = PHYSICAL_ID;
                }
                break;
            case OIS::KC_DOWN:
                notifyHUDNavigation();
                mActionOptionFocused = std::min(3, mActionOptionFocused + 1);
                if (myPartyWaiting.at(myPartyFocused)->info().specialPoints == 0 &&
                        mActionOptionFocused == SPECIAL_ID) {
                    mActionOptionFocused = ITEMS_ID;
                }
                if (mInventory.items.size() == 0 && mActionOptionFocused == ITEMS_ID) {
                    mActionOptionFocused = GUARD_ID;
                }
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
                case OIS::KC_DOWN: {
                    notifyHUDNavigation();
                    mItemsFocused = false;
                    auto itemFrame = mMenuRoot->getChild("Item_Frame");
                    itemFrame->getChild("Items_select")->hide();
                    itemFrame->getChild("Back_select")->show();
                    itemFrame->getChild("Back_select")->activate();
                    break;
                }
                case OIS::KC_LEFT:
                    notifyHUDNavigation();
                    mInventory.cycleInventoryBackward();
                    updateItemBox();
                    break;
                case OIS::KC_RIGHT:
                    notifyHUDNavigation();
                    mInventory.cycleInventoryForward();
                    updateItemBox();
                    break;
                case OIS::KC_RETURN:
                    notifyHUDOptionSelect(); 
                    switchToTargetMenu();
                    break;
                default:
                    return;
            }
            mMenuRoot->getChild("Item_Frame")->getChild("Items_StaticText")->setText(
                    mInventory.getCurrentItemName());
        }
        else {
            switch(arg.key) {
                case OIS::KC_RETURN:
                    mItemRoot->setVisible(false);
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
        auto& targetParty = getTargetParty();
        switch(arg.key) {
            case OIS::KC_Q:
                notifyHUDOptionSelect();
                setTargetArrowVisible(targetParty.at(activeTarget), false);
                activeTarget = 0;
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
                        mItemRoot->setVisible(false);
                        break;
                    default:
                        break;
                }
                dequeueActiveCharacter();
                setTargetArrowVisible(targetParty.at(activeTarget), false);
                activeTarget = 0;
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
    mMenuRoot->getChild("Item_Frame")->getChild("Items_StaticText")->setText(mInventory.getCurrentItemName());
    mItemRoot->setVisible(true);
    updateItemBox();
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

    activeTarget = 0;
    setTargetArrowVisible(getTargetParty().at(activeTarget), true);
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
    if (character->info().specialPoints == 0) {
        mMenuRoot->getChild("Menu_Frame")->getChild("Special_label")->disable();
    }
    else {
        mMenuRoot->getChild("Menu_Frame")->getChild("Special_label")->enable();
    }
}

void HUD::setTargetArrowVisible(Player* character, bool visible) {
    characterTargetArrows.find(character)->second->setVisible(visible);
}

void HUD::cycleActiveCharacter(void) {
    myPartyFocused = (myPartyFocused + 1) % myPartyWaiting.size();
    updateFocusedCharacter(myPartyWaiting.at(myPartyFocused));
    mActionOptions[mActionOptionFocused]->hide();
    mActionOptionFocused = 0;
    mActionOptions[0]->show();
    mActionOptions[0]->activate();

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

void HUD::cycleTargetCharacter() {
    auto& targetParty = getTargetParty();
    setTargetArrowVisible(targetParty.at(activeTarget), false);
    activeTarget = (activeTarget + 1) % targetParty.size();
    setTargetArrowVisible(targetParty.at(activeTarget), true);
}

void HUD::update(void) {
    for(auto character : myParty) {
        auto infoWindow = characterInfoWindows.find(character)->second;
        const PlayerInfo& info = character->info();
        std::string hpText = to_string(info.health) + "/" + to_string(info.healthMax);
        infoWindow->getChild("PM_HP_Value")->setText(hpText);
        std::string spText = to_string(info.specialPoints) + "/" + to_string(info.specialPointsMax);
        infoWindow->getChild("PM_SP_Value")->setText(spText);
        infoWindow->getChild("PM_Damage_Value")->setText(to_string(info.damage));
        infoWindow->getChild("PM_Armor_Value")->setText(to_string(info.armor));
        infoWindow->getChild("PM_Accuracy_Value")->setText(to_string((int)(info.accuracy*100)) + "%");

        auto playerHealthBarBillboard = characterHealthBars.find(character)->second;
        playerHealthBarBillboard->setDefaultWidth(character->info().health * 10);
    }

    for(auto character : enemyParty) {
        auto enemyHealthBarBillBoard = characterHealthBars.find(character)->second;  
        enemyHealthBarBillBoard->setDefaultWidth(character->info().health * 10);
    }

    if (mInventory.items.size() == 0) {
        mMenuRoot->getChild("Menu_Frame")->getChild("Items_label")->disable();
    }
}


void HUD::updateItemBox(void) {
    mItemRoot->getChild("title")->setText(mInventory.getCurrentItemName());
    mItemRoot->getChild("description")->setText(mInventory.getCurrentItemDescription());
    mItemRoot->getChild("count")->setText(to_string(mInventory.getCurrentItemCount()));
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
    Player* target = enemyParty.at(activeTarget);
    mListener->onHUDPhysicalSelect(attacker, target);
}

void HUD::notifySpecialSelect(void) {
    Player* attacker = myPartyWaiting.at(myPartyFocused);
    Player* target = enemyParty.at(activeTarget);
    mListener->onHUDSpecialSelect(attacker, target);
}

void HUD::notifyItemSelect(void) {
    Player* user = myPartyWaiting.at(myPartyFocused);
    Player* target = getTargetParty().at(activeTarget);
    mListener->onHUDItemSelect(user, target);
}

void HUD::notifyGuardSelect(void) {
    Player* guardingCharacter = myPartyWaiting.at(myPartyFocused);
    mListener->onHUDGuardSelect(guardingCharacter);
}

void HUD::notifyCharacterCycle(void) {
    mListener->onHUDCycleCharacter();
}

void HUD::notifyHUDOptionSelect(void) {
    mListener->onHUDOptionSelect();
}

void HUD::notifyHUDNavigation(void) {
    mListener->onHUDNavigation();
}

void HUD::notifyPlayAgain(void) {
    mListener->onHUDPlayAgain();
}

void HUD::notifyQuit(void) {
    mListener->onHUDQuit();
}

std::vector<Player*>& HUD::getTargetParty(void) {
    return mActionOptionFocused == ITEMS_ID &&
        !mInventory.getCurrentItem().isOffensive ?
        myParty : enemyParty;
}

