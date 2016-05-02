#include "Player.h"
#include <OgreMeshManager.h>
#include <OgreStringConverter.h>
#include <OgreParticle.h>

std::default_random_engine Player::rand_generator;
std::uniform_real_distribution<float> Player::rand_dist(0.f, 1.f);

static int idGenerator = 0;
Player::Player(Ogre::SceneManager* _scnmgr, Ogre::SceneNode* _scnnode,
        const PlayerInfo& i, const Ogre::Vector3& pos)
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

    target = nullptr;
    
    mAnimationController = new AnimationController(mEntity, i.mesh.animationSpec); 
    mAnimationController->runIdleAnimation();
}

void Player::physicalAttack(Player& target) {
    int& targetHealth = target.mInfo.health;
    targetHealth -= std::max(0, mInfo.damage - target.mInfo.armor); 
    if (targetHealth < 0) {
        targetHealth = 0;
    }
}

void Player::item(void) {
}

bool Player::attemptPhysicalAttack(void) {
    float roll = rand_dist(rand_generator);
    return roll <= mInfo.accuracy;
}

void Player::guard(void) {
    mInfo.armor += std::max(1, (int)(0.5f * mInfo.armor));
}

void Player::unguard(void) {
    mInfo.armor = mInfo.baseArmor;
}

void Player::specialAttack(Player& target) {
    (void) target;
    --mInfo.specialPoints;
    int randNum = rand() % 3;
    std::cout << randNum << std::endl << std::endl << std::endl;
    ParticleType pt;
    switch(randNum) {
        case 0:
            pt = ParticleType::Fire;
            break;
        case 1:
            pt = ParticleType::Ice;
            break;
        case 2:
            pt = ParticleType::Flare;
            break;
        default:
            break;
    }
    (void) pt;
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
    target = targetObject;
    GameObject::lookAt(targetObject);
    auto targetNode = targetObject->sceneNode;
    for(auto particleNodePair : mParticleNodeMap) {
        auto psNode = particleNodePair.second;
        psNode->lookAt(psNode->convertWorldToLocalPosition(targetNode->_getDerivedPosition() + Ogre::Vector3(0, 100, 0)),
            Ogre::Node::TransformSpace::TS_LOCAL, Ogre::Vector3::NEGATIVE_UNIT_Z);
    }
}

/*
void Player::checkTime(void) {
    if(time(&timer) > itemStartTime + 1.5) {
        setEmitting(ParticleType::Item, false);
        setVisible(ParticleType::Item, false);
    }
    if(time(&timer) > physicalStartTime + 1.0) {
        setEmitting(ParticleType::Physical, false);
        setVisible(ParticleType::Physical, false);
    }
}

void Player::collision(SoundBank* soundBank) {
    for(auto particleSystemPair : mParticleSystemMap) {
        auto ps = particleSystemPair.second;
        ParticleType particleType = particleSystemPair.first;
        if(ps->isVisible() && (particleType == ParticleType::Fire || particleType == ParticleType::Ice || particleType == ParticleType::Flare)) {
            int numParticles = ps->getNumParticles();
            for(int particleNum = 0; particleNum < numParticles; ++particleNum) {
                auto p = ps->getParticle(particleNum);
                if(p) {
                    auto pPos = p->position;
                    auto sn = ps->getParentSceneNode();
                    auto orientation = sn->_getDerivedOrientation();
                    auto snPos = sn->_getDerivedPosition();
                    auto direction = orientation * pPos;
                    auto pWorldPos = snPos + direction;
                    if(target) {
                        auto targetBoundingBox = target->sceneNode->_getWorldAABB();
                        bool hit = targetBoundingBox.intersects(pWorldPos);
                        if(hit) {
                            if(particleNum == numParticles - 1) {
                                setVisible(particleSystemPair.first, false);
                                emittingParticles = false;
                                std::uniform_int_distribution<int> bonus_dist(0, mInfo.accuracy * mInfo.damage);
                                int dmgBonus = bonus_dist(rand_generator);
                                int totalDamage = std::max(0, mInfo.damage + dmgBonus - target->mInfo.armor);
                                target->mInfo.health = std::max(target->mInfo.health - totalDamage, 0);
                                printf("%s hits %s for %d dmg with a bonus of %d\n",
                                        mInfo.name.c_str(), target->mInfo.name.c_str(),
                                        totalDamage, dmgBonus);

                                fflush(stdout);

                                std::cout << "health left " << target->mInfo.health << std::endl; 
                            }
                            setEmitting(particleSystemPair.first, false);

                            // play sound
                            if(particleSystemPair.first == ParticleType::Fire) {
                                soundBank->play("fireball_attack_fx");
                            }
                            if(particleSystemPair.first == ParticleType::Ice) {
                                soundBank->play("ice_attack_fx");
                            }
                            if(particleSystemPair.first == ParticleType::Flare) {
                                soundBank->play("flare_attack_fx");
                            }
                        }
                    }
                }
            }
        }
    }
}
*/

Ogre::Real Player::getHeight(void) {
    return mEntity->getBoundingBox().getSize().y * 1.2;
}
