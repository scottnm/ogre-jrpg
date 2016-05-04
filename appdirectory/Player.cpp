#include "Player.h"
#include <OgreMeshManager.h>
#include <OgreStringConverter.h>
#include <OgreParticle.h>

std::default_random_engine Player::rand_generator;
std::uniform_real_distribution<float> Player::rand_dist(0.f, 1.f);

static int idGenerator = 0;
Player::Player(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode,
        const PlayerInfo& i, const Ogre::Vector3& pos, SoundBank* soundBank)
    : GameObject(_scnmgr), mInfo(i) {

    const Ogre::String id = Ogre::StringConverter::toString(idGenerator++);

    // create the base representable object

	mEntity = _scnmgr->createEntity(i.mesh.name + ".mesh");
	mEntity->setCastShadows(true);
	sceneNode = _scnnode->createChildSceneNode("Player" + id);
	sceneNode->attachObject(mEntity);
    sceneNode->setPosition(pos);
    sceneNode->lookAt(Ogre::Vector3::ZERO, Ogre::Node::TS_WORLD);
    sceneNode->lookAt(Ogre::Vector3(0, pos.y, pos.z), Ogre::Node::TS_WORLD);

    mAnimationController = new AnimationController(mEntity, i.mesh.animationSpec); 
    mAnimationController->runIdleAnimation();

    mParticleController = new ParticleController(_scnmgr, sceneNode, soundBank);

    mDamageIndicatorController = new DamageIndicatorController(_scnmgr, sceneNode);
}

void Player::physicalAttack(Player& target) {
    std::cout << "!!!contact" << std::endl;
    int& targetHealth = target.mInfo.health;
    int damageDealt = std::max(0, mInfo.damage - target.mInfo.armor);
    targetHealth -= damageDealt;
    if (targetHealth < 0) {
        targetHealth = 0;
    }
    target.mDamageIndicatorController->alertDamage(damageDealt);
    std::cout << "finish" << std::endl;
}

void Player::missAttack(Player& target) {
    std::cout << "!!!miss" << std::endl;
    target.mDamageIndicatorController->alertMiss();
}

void Player::item(void) {
}

bool Player::attemptPhysicalAttack(void) {
    float roll = rand_dist(rand_generator);
    return roll <= mInfo.accuracy;
}

void Player::guard(void) {
    mInfo.armor += std::max(1, (int)(0.5f * mInfo.armor));
    mParticleController->enableGuard();
}

void Player::unguard(void) {
    mInfo.armor = mInfo.baseArmor;
    mParticleController->disableGuard();
}

void Player::specialAttack(Player& target) {
    --mInfo.specialPoints;
    std::uniform_int_distribution<int> bonus_dist(0, mInfo.accuracy * mInfo.damage);
    int dmgBonus = bonus_dist(rand_generator);
    int totalDamage = std::max(0, mInfo.damage + dmgBonus - target.mInfo.armor);
    target.mInfo.health = std::max(target.mInfo.health - totalDamage, 0);
    target.mDamageIndicatorController->alertDamage(totalDamage);
}

bool Player::isDead(void) {
    return mInfo.health == 0;
}

void Player::reset(void) {
    mInfo.reset();
}

const PlayerInfo& Player::info(void) const {
    return mInfo;
}

PlayerInfo& Player::info(void) {
    return mInfo;
}

void Player::lookAt(Player* targetObject) {
    GameObject::lookAt(targetObject);
    auto targetNode = targetObject->sceneNode;
    for(auto particleNodePair : mParticleNodeMap) {
        auto psNode = particleNodePair.second;
        psNode->lookAt(psNode->convertWorldToLocalPosition(targetNode->_getDerivedPosition() + Ogre::Vector3(0, 100, 0)),
            Ogre::Node::TransformSpace::TS_LOCAL, Ogre::Vector3::NEGATIVE_UNIT_Z);
    }
}

Ogre::Real Player::getHeight(void) {
    return mEntity->getBoundingBox().getSize().y * 1.2;
}
