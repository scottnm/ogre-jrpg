#ifndef __HUD_H__
#define __HUD_H__

#include "GUISystem.h"
#include "HUDListener.h"
#include "Player.h"
#include "Inventory.h"
#include "Item.h"

#include <OgreBillboardSet.h>
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
    HUD(Ogre::SceneManager& scnMgr,
        GUISystem& gui,
        HUDListener* listener,
        std::vector<Player*>& myParty,
        std::vector<Player*>& enemyParty,
        std::vector<Player*>& myPartyWaiting,
        Inventory& inventory);
    ~HUD(void);

    void injectKeyDown(const OIS::KeyEvent& arg);
    void update(void);
    void refocusAfterCharacterDeath(void);
    void alertGameOver(bool userWins);


private:
    void switchToItemMenu(void);
    void switchToActionMenu(void);
    void switchToTargetMenu(void);

    void cycleActiveCharacter(void);
    void dequeueActiveCharacter(void);
    void cycleTargetCharacter(void);

    void updateFocusedCharacter(Player* character);
    void setTargetArrowVisible(Player* character, bool visible);
    void updateItemBox(void);

    void notifyPhysicalSelect(void);
    void notifySpecialSelect(void);
    void notifyItemSelect(void);
    void notifyGuardSelect(void);
    void notifyCharacterCycle(void);
    void notifyHUDOptionSelect(void);
    void notifyHUDNavigation(void);
    void notifyPlayNext(void);
    void notifyQuit(void);

    std::vector<Player*>& getTargetParty(void);

    GUISystem& mGUI;
    HUDListener* mListener;
    HUD_STATE mState;
    HUD_STATE mPrevState;
    CEGUI::Window* mMenuRoot;
    CEGUI::Window* mEndStateRoot;
    CEGUI::Window* mItemRoot;

    // action menu
    int mActionOptionFocused; // mActionOptionFocused
    CEGUI::Window* mActionOptions[4]; // mActionOptions

    // item menu
    bool mItemsMenuVisible; 
    bool mItemsFocused;

    // end game
    bool mPlayNextOptionFocused;

    std::vector<Player*>& myParty;
    std::vector<Player*>& myPartyWaiting;
    long unsigned int myPartyFocused;
    CEGUI::Window* charSelected;
    std::unordered_map<Player*, CEGUI::Window*> characterInfoWindows;
    std::unordered_map<Player*, Ogre::SceneNode*> characterTargetArrows;
    std::unordered_map<Player*, Ogre::BillboardSet*> characterHealthBars;
    std::vector<Player*>& enemyParty;
    int activeTarget;
    // std::vector<std::pair<Item,int>>& items;
    Inventory& mInventory;

    static const Ogre::String windowName;
    static const Ogre::String mockItems[4];
};

#endif // __HUD_H__
