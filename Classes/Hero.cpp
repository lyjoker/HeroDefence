//
//  Hero.cpp
//  HeroDeffence
//
//  Created by Yuan Lin on 3/5/14.
//
//

#include "Hero.h"
#include "GameMap.h"
#include "AnimationUtil.h"
#include "GameScene.h"
#include "Bullet.h"


USING_NS_CC;

bool Hero::initWithProperty(const char* pName, int pHP, int pSpeed, int pAttack, int pLine, float pX, float pAttSpeed, int pAttRange)
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
                                                                            StringUtils::format("%s_wait1.png", name.c_str())
                                                                            )
                                           );
    sprite->setAnchorPoint(Point(0.5, 0));
    sprite->setFlippedX(!direction);
    this->setPosition(position);
    this->addChild(sprite);
    
    healthBar = ProgressTimer::create(Sprite::create("health_bar_green.png"));
    healthBar->setType(ProgressTimer::Type::BAR);
    healthBar->setMidpoint(Point(0, 0));
    healthBar->setBarChangeRate(Point(1, 0));
    healthBar->setPercentage(100);
    healthBar->setScale(sprite->getContentSize().width / healthBar->getContentSize().width);
    healthBar->setScaleY(healthBar->getScaleY()*1.5f);
    healthBar->setAnchorPoint(Point(0.5,0));
    healthBar->setPosition(Point(0, sprite->getContentSize().height+10));
    this->addChild(healthBar, 2);
    auto redbar = Sprite::create("health_bar_red.png");
    redbar->setPosition(healthBar->getPosition());
    redbar->setScale(sprite->getContentSize().width / redbar->getContentSize().width);
    redbar->setScaleY(redbar->getScaleY()*1.5f);
    redbar->setAnchorPoint(Point(0.5,0));
    this->addChild(redbar, 1);
    
    schedule(schedule_selector(Hero::heroUpdate), 0.1f);
    
    this->setDefaultProperty();
    
    status = STATUS_DONOTHING;
    type = TYPE_HERO;
    GameScene::playerList->pushBack(this);
    animateWait();
    
    return true;
}
void Hero::setDamage(int damage, Entity * who)
{
    curHP -= damage;
    healthBar->setPercentage(float(curHP) / float(maxHP) * 100);
    if (curHP <= 0 && active)
    {
        setDead();
    }
}
void Hero::setDead()
{
    active = false;
    //GameScene::playerList->eraseObject(this);
    //Animate* animate = Animate::create(AnimationUtil::createAnimWithFrame(StringUtils::format("%s_dead", name.c_str()), 0.08f, 1));
    //this->runAction(Sequence::create(animate,
      //                               CallFunc::create(CC_CALLBACK_0(Hero::dead, this)),
       //                              NULL));
    status = STATUS_DEAD;
    dead();
}
void Hero::dead()
{
    this->setVisible(false);
    this->unscheduleAllSelectors();
    this->scheduleOnce(schedule_selector(Hero::reborn), 5);
    ((GameScene*)getParent())->heroDead();
}
void Hero::reborn(float dt)
{
    curHP = maxHP;
    Point position = Point(0, GameMap::lineToY(2));
    this->setPosition(position);
    healthBar->setPercentage(100);
    status = STATUS_DONOTHING;
    direction = false;
    animateWait();
    sprite->runAction(FlipX::create(true));
    schedule(schedule_selector(Hero::heroUpdate), 0.1f);
    active = true;
    this->setVisible(true);
}
void Hero::animateRun()
{
    
    Animate* animate = Animate::create(AnimationUtil::createAnimWithFrame(StringUtils::format("%s_run", name.c_str()), 100.0f/runFrames/speed, -1));
    if (animate == NULL) return;
    sprite->stopAllActions();
    sprite->runAction(animate);
}
void Hero::animateAttack()
{
    
    Animate* animate = Animate::create(AnimationUtil::createAnimWithFrame(StringUtils::format("%s_attack", name.c_str()), 0.7f/attFrames/attSpeed, 1));
    if (animate == NULL) return;
    sprite->stopAllActions();
    sprite->runAction(animate);
    
}
void Hero::animateWait()
{
    Animate* animate = Animate::create(AnimationUtil::createAnimWithFrame(StringUtils::format("%s_wait", name.c_str()), 0.08f, -1));
    if (animate == NULL) return;
    sprite->stopAllActions();
    sprite->runAction(animate);
}

void Hero::attackObject(Entity* target)
{
    target->setDamage(attack, this);
}
void Hero::heroUpdate(float dt)
{
    if (status == STATUS_COOLDOWN || status == STATUS_RUNNING)
        return;
    if (!active || hasRemoved)
        return;
    Entity* attObject = NULL;
    float minDist = 999999.0f;
    for( Entity* object : *GameScene::enemyList)
    {
        if (object==NULL || object->hasRemoved) continue;
        if (object->getLine() != line || object->getHP()<=0)
            continue;
        float tmpDist = this->getMidPoint().getDistance(object->getMidPoint());
        if ((attRange==0 && this->getEffectRect().intersectsRect(object->getEffectRect()))
            || (attRange>0 && tmpDist<=attRange && tmpDist<minDist))
        {
            attObject = object;
            minDist = tmpDist;
            break;
        }
    }
    if (attObject==NULL && status==STATUS_ATTACKING)
    {
        status = STATUS_DONOTHING;
        animateWait();
        return;
    }
    if (attObject!=NULL)
    {
        this->stopAllActions();
        
        this->animateAttack();
        if ((attObject->getMidPoint().x>this->getMidPoint().x && direction) ||
            (attObject->getMidPoint().x<this->getMidPoint().x && !direction))
        {
            sprite->runAction(FlipX::create(direction));
            direction = !direction;
        }
        status = STATUS_COOLDOWN;
        this->runAction(Sequence::create(
                                         DelayTime::create(0.4f/attSpeed),
                                         CallFunc::create([=](){
            attackObject(attObject);
        }),
                                         DelayTime::create(0.6f/attSpeed),
                                         CallFunc::create([&](){
            status = STATUS_ATTACKING;
        }),
                                         NULL
                                         ));
        
    }
    
}
void Hero::runToDest(Point _dest)
{
    if (status == STATUS_DEAD)
        return;
    if (fabs(_dest.y-this->getPositionY())<150)
    {
        dest.y = this->getPositionY();
        dest.x = _dest.x;
        this->stopAllActions();
        status = STATUS_RUNNING;
        animateRun();
        if ((direction && dest.x>this->getPositionX())
            || (!direction && dest.x<this->getPositionX()))
        {
            auto flip = FlipX::create(direction);
            sprite->runAction(flip);
            direction = !direction;
        }
        auto moveto = MoveTo::create(fabs(dest.x-this->getPositionX()) / speed, dest);
        this->runAction(Sequence::create(moveto,
                                         CallFunc::create([&](){status = STATUS_DONOTHING; animateWait();}),
                                         NULL));
    }
}
HeroCat* HeroCat::create()
{
    HeroCat* pRet = new HeroCat();
    if (pRet && pRet->initWithProperty("Hero_Cat", 1500, 200, 100, 2, 0, 0.7f, 400))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}
void HeroCat::setDefaultProperty()
{
    pointWidthRate = 0.4f;
    pointHeightRate = 0.0f;
    widthRate = 0.2f;
    heightRate = 0.8f;
    runFrames = 8;
    attFrames = 12;
}
void HeroCat::attackObject(Entity *target)
{
    if (target==NULL || target->hasRemoved || target->getHP()<=0)
        return;
    Point tmp = this->getMidPoint();
    if (direction)
        tmp.x -= sprite->getContentSize().width / 2;
    else
        tmp.x += sprite->getContentSize().width / 2;
    BulletIntracing *bullet = BulletIntracing::create(target, attack,  tmp, "Bullet_GreenBall", false, 1.0f, PLAYER_FACTION, 500, this);
    bullet->fire();
    this->getParent()->addChild(bullet, 3);
}
