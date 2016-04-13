#ifndef __HUD_H__
#define __HUD_H__

#include "GUISystem.h"

#include <OgreString.h>

#include <CEGUI/CEGUI.h>

class HUD {
public:
    HUD(GUISystem& gui);

    GUISystem& mGUI;
    CEGUI::Window* mRoot;
    int mOptionSelected;
    bool mItemsMenuVisible; 

    static const Ogre::String windowName;
};

#endif // __HUD_H__
