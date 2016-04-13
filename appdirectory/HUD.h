#ifndef __HUD_H__
#define __HUD_H__

#include "GUISystem.h"

#include <OgreString.h>
#include <OISKeyboard.h>
#include <CEGUI/CEGUI.h>

class HUD {
public:
    HUD(GUISystem& gui);
    void injectKeyDown(const OIS::KeyEvent& arg);
    void injectKeyUp(const OIS::KeyEvent& arg);

    GUISystem& mGUI;
    CEGUI::Window* mRoot;
    int mOptionSelected;
    CEGUI::Window* mOptionSelects[4];

    bool mItemsMenuVisible; 
    bool mItemsFocused;
    int mItemsTotal;
    int mItemSelected;

    static const Ogre::String windowName;
    static const Ogre::String mockItems[4];
};

#endif // __HUD_H__
