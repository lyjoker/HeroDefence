//
//  Soldier.cpp
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#include "Soldier.h"
#include "GameMap.h"
#include "AnimationUtil.h"
#include "GameScene.h"
#include "Bullet.h"

USING_NS_CC;

bool Soldier::initWithProperty(const char* pName, int pHP, int pSpeed, int pAttack, int pLine, float pX, float pAttSpeed, int pAttRange)
{
    hasRemoved = false;
    name = pName;
    curHP = maxHP = pHP;
    speed = pSpeed;
    attack = pAttack;
    Point position = Point(pX, GameMap::lineToY(pLine));
    line = pLine;
    attSpeed = pAttSpeed;
    attRange = pAttRange;
    active = true;
    direction = false;
    auto frameCache = SpriteFrameCache::getInstance();
    sprite = Sprite::createWithSpriteFrame(
                                           frameCache->getSpriteFrameByName(
                                                                            StringUtils::format("%s_run1.png", name.c_str())
                                                                            )
                                           );
    sprite->setAnchorPoint(Point(0, 0));
    sprite->setFlippedX(direction);
    this->setPosition(position);
    this->addChild(sprite);
    status = STATUS_RUNNING;
    endPoint = position;
    endPoint.x = RIGHT_EDGE_X;
    
    healthBar = ProgressTimer::create(Sprite::create("health_bar_green.png"));
    healthBar->setType(ProgressTimer::Type::BAR);
    healthBar->setMidpoint(Point(0, 0));
    healthBar->setBarChangeRate(Point(1, 0));
    healthBar->setPercentage(100);
    healthBar->setScale(sprite->getContentSize().width / healthBar->getContentSize().width);
    healthBar->setAnchorPoint(Point(0,0));
    healthBar->setPosition(Point(0, sprite->getContentSize().height));
    this->addChild(healthBar, 2);
    auto redbar = Sprite::create("health_bar_red.png");
    redbar->setPosition(healthBar->getPosition());
    redbar->setScale(sprite->getContentSize().width / redbar->getContentSize().width);
    redbar->setAnchorPoint(Point(0,0));
    this->addChild(redbar, 1);
    
    schedule(schedule_selector(Soldier::soldierUpdate), 0.1f);
    
    this->setDefaultProperty();
    
    
    
    GameScene::playerList->pushBack(this);
    this->runToDest();
    
    return true;
}
void Soldier::setDamage(int damage, Entity * who)
{
    curHP -= damage;
    healthBar->setPercentage(float(curHP) / float(maxHP) * 100);
    if (curHP <= 0 && active)
    {
        setDead();
    }
}
void Soldier::setDead()
{
    active = false;
    GameScene::playerList->eraseObject(this);
    this->stopAllActions();
    sprite->stopAllActions();
    auto dead1 = Blink::create(0.3f, 3);
    this->runAction(Sequence::create(
                                     dead1,
                                     CallFunc::create(
                                                      [&](){removeSelf();}
                                                      ),
                                     NULL));
}

void Soldier::animateRun()
{
    
    Animate* animate = Animate::create(AnimationUtil::createAnimWithFrame(StringUtils::format("%s_run", name.c_str()), 60.0f/runFrames/speed, -1));
    if (animate == NULL) return;
    sprite->stopAllActions();
    sprite->runAction(animate);
}
void Soldier::animateAttack()
{
    
    Animate* animate = Animate::create(AnimationUtil::createAnimWithFrame(StringUtils::format("%s_attack", name.c_str()), 0.5f/attFrames/attSpeed, 1));
    if (animate == NULL) return;
    sprite->stopAllActions();
    sprite->runAction(animate);
    
}
void Soldier::attackObject(Entity* target)
{
    target->setDamage(attack, this);
}
void Soldier::soldierUpdate(float dt)
{
    if (status == STATUS_COOLDOWN)
        return;
    if (!active || hasRemoved)
        return;
    Entity* attObject = NULL;
    for( Entity* object : *GameScene::enemyList)
    {
        if (object==NULL || object->hasRemoved) continue;
        if (object->getLine() != line || object->getHP()<=0)
            continue;
        if ((attRange==0 && this->getEffectRect().intersectsRect(object->getEffectRect()))
            || (attRange>0 && this->getPosition().getDistance(object->getMidPoint())<=attRange))
        {
            attObject = object;
            break;
        }
    }
    if (attObject==NULL && status==STATUS_ATTACKING)
    {
        status = STATUS_RUNNING;
        this->runToDest();
        return;
    }
    if (attObject!=NULL)
    {
        this->stopAllActions();
        this->animateAttack();
        this->attackObject(attObject);
        status = STATUS_COOLDOWN;
        this->runAction(Sequence::create(
                                         DelayTime::create(0.5f/attSpeed),
                                         CallFunc::create([&](){
            status = STATUS_ATTACKING;
        }),
                                         NULL
                                         ));
        
    }
    
}
void Soldier::runToDest()
{
    float dist = abs(this->getPosition().x - endPoint.x);
    auto moveto = MoveTo::create(dist/speed, endPoint);
    this->animateRun();
    this->runAction(moveto);
}
SoldierFox* SoldierFox::create(int pLine, float pX)
{
    SoldierFox *pRet = new SoldierFox();
    if (pRet && pRet->initWithProperty("Soldier_Fox", 1500, 80, 60, pLine, pX, 1.1f, 0))
    {
        pRet->autorelease();
        return pRet;
    }else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}
void SoldierFox::setDefaultProperty()
{
    pointWidthRate = 0.15f;
    pointHeightRate = 0.15f;
    widthRate = 0.9f;
    heightRate = 0.7f;
    runFrames = 6;
    attFrames = 6;
}