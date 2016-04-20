#ifndef __HUD_H__
#define __HUD_H__

#include "GUISystem.h"
#include "HUDListener.h"
#include "HUDTargetable.h"
#include "Player.h"

#include <OgreString.h>
#include <OISKeyboard.h>
#include <CEGUI/CEGUI.h>

#include <vector>

enum class HUD_STATE {
    ACTION_MENU_ACTIVE,
    ITEMS_MENU_ACTIVE,
    TARGETING_MENU_ACTIVE
};

class HUD {
public:
    HUD(GUISystem& gui, HUDListener& listener,
        std::vector<Player*>& myParty, std::vector<Player*>& enemyParty,
        std::vector<Player*>& myPartyWaiting);
    ~HUD(void);

    void injectKeyDown(const OIS::KeyEvent& arg);
    void injectKeyUp(const OIS::KeyEvent& arg);
    void updateFocusedCharacter(int characterId);

    static const Ogre::String windowName;

private:
    void switchToItemMenu(void);
    void switchToActionMenu(void);
    void switchToTargetMenu(void);

    void cycleActiveCharacter(void);
    void dequeueActiveCharacter(void);
    void cycleTargetCharacter(void);

    GUISystem& mGUI;
    HUDListener& mListener;
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
    int myPartyActiveTarget;
    std::vector<Player*>& enemyParty;
    int enemyPartyActiveTarget;

    static const Ogre::String mockItems[4];
};

#endif // __HUD_H__
