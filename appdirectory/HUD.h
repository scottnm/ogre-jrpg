#ifndef __HUD_H__
#define __HUD_H__

#include "GUISystem.h"
#include "HUDListener.h"
#include "Player.h"
#include "Inventory.h"
#include "Item.h"

#include <OgreString.h>
#include <OgreSceneManager.h>
#include <OISKeyboard.h>
#include <CEGUI/CEGUI.h>

#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>

enum class HUD_STATE {
    ACTION_MENU_ACTIVE,
    ITEMS_MENU_ACTIVE,
    TARGETING_MENU_ACTIVE,
    GAME_OVER
};

class HUD {
public:
    HUD(Ogre::SceneManager& scnMgr, GUISystem& gui, std::vector<Player*>& myParty,
            std::vector<Player*>& enemyParty, std::vector<Player*>& myPartyWaiting,
            Inventory& inventory);
    ~HUD(void);

    void injectKeyDown(const OIS::KeyEvent& arg);
    void update(void);
    void registerListener(HUDListener* hl);
    void updateFocusedCharacter(Player* character);
    void setTargetArrowVisible(Player* character, bool visible);
    void refocusAfterCharacterDeath(void);
    void alertGameOver(bool userWins);

    static const Ogre::String windowName;

private:
    void switchToItemMenu(void);
    void switchToActionMenu(void);
    void switchToTargetMenu(void);

    void cycleActiveCharacter(void);
    void dequeueActiveCharacter(void);
    void cycleTargetCharacter(void);

    void notifyPhysicalSelect(void);
    void notifySpecialSelect(void);
    void notifyItemSelect(void);
    void notifyGuardSelect(void);
    void notifyCharacterCycle(void);
    void notifyHUDOptionSelect(void);
    void notifyHUDNavigation(void);
    void notifyPlayAgain(void);
    void notifyQuit(void);

    GUISystem& mGUI;
    std::unordered_set<HUDListener*> mListeners;
    CEGUI::Window* mMenuRoot;
    CEGUI::Window* mEndStateRoot;
    HUD_STATE mState;
    HUD_STATE mPrevState;

    // action menu
    int mActionOptionFocused; // mActionOptionFocused
    CEGUI::Window* mActionOptions[4]; // mActionOptions

    // item menu
    bool mItemsMenuVisible; 
    bool mItemsFocused;

    // end game
    bool mPlayAgainOptionFocused;

    std::vector<Player*>& myParty;
    std::vector<Player*>& myPartyWaiting;
    long unsigned int myPartyFocused;
    CEGUI::Window* charSelected;
    std::unordered_map<Player*, CEGUI::Window*> characterInfoWindows;
    std::unordered_map<Player*, Ogre::SceneNode*> characterTargetArrows;
    std::vector<Player*>& enemyParty;
    int enemyPartyActiveTarget;
    // std::vector<std::pair<Item,int>>& items;
    Inventory& inventory;

    static const Ogre::String mockItems[4];
};

#endif // __HUD_H__
