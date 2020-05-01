#pragma once

#include <OISMouse.h>
#include <OISKeyboard.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include <string>
#include <unordered_map>

class GUISystem : public OIS::KeyListener,
                  public OIS::MouseListener {
public:
    GUISystem(void);
    ~GUISystem(void);

    void addNewWindowGroup(std::string name, CEGUI::Window* root);
    void switchToWindowGroup(std::string name);
    void addAndSetWindowGroup(std::string name, CEGUI::Window* root);
    CEGUI::Window* retrieveWindowGroup(std::string name);
    void destroyWindowGroup(std::string _n);

    virtual bool keyPressed(const OIS::KeyEvent& arg);
    virtual bool keyReleased(const OIS::KeyEvent& arg);
    virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
    virtual bool mouseMoved(const OIS::MouseEvent& arg);

private:
    // use a map to keep track of different "roots"
    // (roots = gui roots / menus / interfaces / whatever
    std::unordered_map<std::string, CEGUI::Window*> mWindowDatabase;
    CEGUI::OgreRenderer* mGUIRenderer;
    CEGUI::GUIContext* mContext;
    CEGUI::Window* mRootWindow;
    CEGUI::Window* mActiveWindow;
};
