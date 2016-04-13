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

    static const Ogre::String windowName;
};

#endif // __HUD_H__
