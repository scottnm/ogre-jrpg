#ifndef _WIN32 // TODO: get to compile

#include <DamageIndicatorController.h>
#include <OgreBillboardSet.h>
#include <OgreMath.h>
#include <OgreStringConverter.h>
using Ogre::StringConverter;

Ogre::Vector3 DamageIndicatorController::endPos(0, 250, 0);

DamageIndicatorController::DamageIndicatorController(Ogre::SceneManager* scnMgr,
        Ogre::SceneNode* cRoot) : mSceneManager(scnMgr), mCharacterRoot(cRoot) {}

void DamageIndicatorController::alertDamage(int dmg) {
    auto damageBillboardSet = mSceneManager->createBillboardSet();
    damageBillboardSet->setMaterialName("damage_" + StringConverter::toString(dmg, 2, '0',
                std::ios_base::dec | std::ios_base::right));
    damageBillboardSet->createBillboard(Ogre::Vector3::ZERO);
    damageBillboardSet->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAX);
    damageBillboardSet->setDefaultDimensions(40, 40);

    auto damageNode = mCharacterRoot->createChildSceneNode(Ogre::Vector3(0, 200, 0));
    damageNode->attachObject(damageBillboardSet);

    mIndicators.push_back(DamageIndicator(damageNode, 1.0f));
}

void DamageIndicatorController::alertMiss(void) {
    auto missBillboardSet = mSceneManager->createBillboardSet();
    missBillboardSet->setMaterialName("damage_miss");
    missBillboardSet->createBillboard(Ogre::Vector3::ZERO);
    missBillboardSet->setRenderQueueGroup(Ogre::RENDER_QUEUE_MAX);
    missBillboardSet->setDefaultDimensions(40, 40);

    auto missNode = mCharacterRoot->createChildSceneNode(Ogre::Vector3(0, 200, 0));
    missNode->attachObject(missBillboardSet);

    mIndicators.push_back(DamageIndicator(missNode, 1.0f));
}

void DamageIndicatorController::injectTime(Ogre::Real secondsElapsed) {
    auto i = mIndicators.begin();
    while (i != mIndicators.end()) {
        DamageIndicator& di = *i;
        if (/* time over */ di.second <= secondsElapsed) {
            // hide the scene node
            di.first->setVisible(false);
            // remove it from the indicator list
            i = mIndicators.erase(i);
        }
        else {
            // update the time of the indicator
            di.second -= secondsElapsed;
            // translate the indicator
            di.first->setPosition(
                    Ogre::Math::lerp(di.first->getPosition(), endPos, secondsElapsed));
            ++i;
        }
    }
}

#endif // _WIN32
