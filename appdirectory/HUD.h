#ifndef __HUD_H__
#define __HUD_H__

#include "GUISystem.h"
#include "HUDListener.h"
#include "Player.h"

#include <OgreString.h>
#include <OgreSceneManager.h>
#include <OISKeyboard.h>
#include <CEGUI/CEGUI.h>

#include <unordered_set>
#include <unordered_map>
#include <vector>

enum class HUD_STATE {
    ACTION_MENU_ACTIVE,
    ITEMS_MENU_ACTIVE,
    TARGETING_MENU_ACTIVE
};

class HUD {
public:
    HUD(Ogre::SceneManager& scnMgr, GUISystem& gui, std::vector<Player*>& myParty,
            std::vector<Player*>& enemyParty, std::vector<Player*>& myPartyWaiting);
    ~HUD(void);

    void injectKeyDown(const OIS::KeyEvent& arg);
    void update(void);
    void registerListener(HUDListener* hl);

    static const Ogre::String windowName;

private:
    void switchToItemMenu(void);
    void switchToActionMenu(void);
    void switchToTargetMenu(void);

    void cycleActiveCharacter(void);
    void dequeueActiveCharacter(void);
    void cycleTargetCharacter(void);
    void updateFocusedCharacter(Player* character);
    void setTargetArrowVisible(Player* character, bool visible);

    void notifyPhysicalSelect(void);
    void notifySpecialSelect(void);
    void notifyItemSelect(void);
    void notifyGuardSelect(void);
    void notifyCharacterCycle(void);
    void notifyHUDOptionSelect(void);
    void notifyHUDNavigation(void);

    GUISystem& mGUI;
    std::unordered_set<HUDListener*> mListeners;
    CEGUI::Window* mRoot;
    int mOptionSelected;
    CEGUI::Window* mOptionSelects[4];
    CEGUI::Window* charSelected;

    bool mItemsMenuVisible; 
    HUD_STATE mState;
    HUD_STATE mPrevState;
    bool mItemsFocused;
    int mItemsTotal;
    int mItemSelected;

    std::vector<Player*>& myParty;
    std::vector<Player*>& myPartyWaiting;
    unsigned int myPartyFocused;
    std::unordered_map<Player*, CEGUI::Window*> characterInfoWindows;
    std::unordered_map<Player*, Ogre::SceneNode*> characterTargetArrows;
    std::vector<Player*>& enemyParty;
    int enemyPartyActiveTarget;

    static const Ogre::String mockItems[4];
};

#endif // __HUD_H__
