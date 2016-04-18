#ifndef __HUD_H__
#define __HUD_H__

#include "GUISystem.h"
#include "PlayerInfo.h"

#include <OgreString.h>
#include <OISKeyboard.h>
#include <CEGUI/CEGUI.h>

class HUDListener {
public:
    virtual void onHUDPhysicalSelect() = 0;
    virtual void onHUDSpecialSelect() = 0;
    virtual void onHUDItemSelect() = 0;
    virtual void onHUDGuardSelect() = 0;
};

class HUD {
public:
    HUD(GUISystem& gui, HUDListener& listener,
            const PlayerInfo& p1, const PlayerInfo& p2, const PlayerInfo& p3);
    ~HUD(void);

    void injectKeyDown(const OIS::KeyEvent& arg);
    void injectKeyUp(const OIS::KeyEvent& arg);

    static const Ogre::String windowName;

private:
    void switchToItemMenu(void);
    void switchToActionMenu(void);

    GUISystem& mGUI;
    CEGUI::Window* mRoot;
    int mOptionSelected;
    CEGUI::Window* mOptionSelects[4];

    bool mItemsMenuVisible; 
    bool mItemsFocused;
    int mItemsTotal;
    int mItemSelected;

    HUDListener& mListener;

    static const Ogre::String mockItems[4];
};

#endif // __HUD_H__
