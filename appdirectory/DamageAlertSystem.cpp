#include "DamageAlertSystem.h"
#include <OgreStringConverter.h>
#include <Overlay/OgreFontManager.h>


DamageAlertSystem::DamageAlertSystem(void) {
    Ogre::FontPtr mFont = Ogre::FontManager::getSingleton().create("BlueHighway-8", "Fonts");
    Ogre::FontManager::getSingleton().getByName("BlueHighway-8")->load();
}

DamageAlertSystem::~DamageAlertSystem(void) {
    for(auto& a : mAlerts) {
        delete a.text;
    }
}

void DamageAlertSystem::addAlert(Player* player, int dmgDealt) {
    static int textIdGenerator = 0;
    Ogre::MovableText* text = new Ogre::MovableText(Ogre::String("DMG_ALERT_") +
            Ogre::StringConverter::toString(textIdGenerator++),
            Ogre::StringConverter::toString(dmgDealt), "BlueHighway-16");
    text->setTextAlignment(Ogre::MovableText::H_CENTER, Ogre::MovableText::V_CENTER);
    text->showOnTop(true);
    text->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
    auto node = player->sceneNode->createChildSceneNode(Ogre::Vector3(0, 100, 0));
    //node->attachObject(player->sceneManager->createEntity("sphere.mesh"));
    std::cout << "text: " << text->getCaption() << std::endl;
    node->attachObject(text);
    mAlerts.push_back(DamageAlert(node, text));
}

void DamageAlertSystem::updateAlerts(Ogre::Real secondsElapsed) {
    for(auto alertsItr = mAlerts.begin(); alertsItr != mAlerts.end(); ++alertsItr) {
        alertsItr->secondsElapsed += secondsElapsed;
        if (alertsItr->secondsElapsed > DamageAlertSystem::DISPLAY_TIME) {
            alertsItr->node->detachAllObjects();
            alertsItr = mAlerts.erase(alertsItr);
            if (alertsItr == mAlerts.end()) {break;}
        }
    }
}
