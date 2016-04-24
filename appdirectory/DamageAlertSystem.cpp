#include "DamageAlertSystem.h"
#include <OgreStringConverter.h>
#include <Overlay/OgreFontManager.h>
#include <OgreRay.h>
#include <OgreSceneQuery.h>
#include <OgreCamera.h>
#include "MovableTextOverlay.h"
#include <OgreColourValue.h>
#include <RectLayoutManager.h>


DamageAlertSystem::DamageAlertSystem(Ogre::Camera& c) : mCamera(c) {
    Ogre::FontPtr mFont = Ogre::FontManager::getSingleton().create("BlueHighway-8", "Fonts");
    Ogre::FontManager::getSingleton().getByName("BlueHighway-8")->load();
}

DamageAlertSystem::~DamageAlertSystem(void) {
    for(auto& a : mAlerts) {
    }
}

void DamageAlertSystem::addAlert(Player* player, int dmgDealt) {
    static int textIdGenerator = 0;
    // get terrain height under the camera
    Ogre::RaySceneQuery* raySceneQuery2 = player->sceneManager->createRayQuery(Ogre::Ray(mCamera.getPosition(), Ogre::Vector3::NEGATIVE_UNIT_Y));
    Ogre::RaySceneQueryResult& qryResult = raySceneQuery2->execute();
    Ogre::RaySceneQueryResult::iterator i = qryResult.begin();
    if (i != qryResult.end() && i->worldFragment)
    {       
        // create the attributes used by MovableTextOverlay
        MovableTextOverlayAttributes *attrs = new MovableTextOverlayAttributes("Attrs1", &mCamera, "BlueHighway-16",16, Ogre::ColourValue::White,"RedTransparent");
     
        MovableTextOverlay* p = new MovableTextOverlay("Text"+Ogre::StringConverter::toString(textIdGenerator++), "Test", player->mEntity, attrs);
        p->enable(false); // make it invisible for now
        p->setUpdateFrequency(0.01);// set update frequency to 0.01 seconds
        DamageAlert d;
        d.mto = p;
        d.secondsElapsed = 0;
        mAlerts.push_back(d);
    }
}

void DamageAlertSystem::updateAlerts(Ogre::Real secondsElapsed) {
/*
    for(auto alertsItr = mAlerts.begin(); alertsItr != mAlerts.end(); ++alertsItr) {
        alertsItr->secondsElapsed += secondsElapsed;
        if (alertsItr->secondsElapsed > DamageAlertSystem::DISPLAY_TIME) {
            alertsItr->node->detachAllObjects();
            alertsItr = mAlerts.erase(alertsItr);
            if (alertsItr == mAlerts.end()) {break;}
        }
    }
*/
}

bool DamageAlertSystem::frameStarted(const Ogre::FrameEvent& evt) {
    RectLayoutManager m(0,0,mCamera.getViewport()->getActualWidth(),
        mCamera.getViewport()->getActualHeight());
    m.setDepth(0);
     
    for(auto& d : mAlerts)
    {
        MovableTextOverlay* p = d.mto;
        p->update(evt.timeSinceLastFrame);
        if (p->isOnScreen())
        {
     
            RectLayoutManager::Rect r(  p->getPixelsLeft(),
                                        p->getPixelsTop(),
                                        p->getPixelsRight(),
                                        p->getPixelsBottom());
     
            RectLayoutManager::RectList::iterator it = m.addData(r);
            if (it != m.getListEnd())
            {
                p->setPixelsTop((*it).getTop());
                p->enable(true);
            }
            else
                p->enable(false);
        }
        else
            p->enable(false);
    }
    return true;
}
