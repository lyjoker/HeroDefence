//
//  Tower.cpp
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#include "Tower.h"
#include "GameScene.h"
#include "Bullet.h"
#include "GameMap.h"
#include "AnimationUtil.h"
#include "Entity.h"
#include "Soldier.h"

USING_NS_CC;

bool Tower::initWithProperty(int _curHP, int _maxHP, int _attack, float _attSpeed, int _attRange, float _pX, int _line, std::string _name)
{
    hasRemoved = false;
    curHP = _curHP;
    maxHP = _maxHP;
    attack = _attack;
    attSpeed = _attSpeed;
    Point position = Point(_pX, GameMap::lineToY(_line));
    line = _line;
    attRange = _attRange;
    name = _name;
    active = true;
    auto frameCache = SpriteFrameCache::getInstance();
    
    sprite = Sprite::createWithSpriteFrame(
                                             frameCache->getSpriteFrameByName(
                                                                              StringUtils::format("%s.png", name.c_str())
                                                                              )
                                             );
    sprite->setAnchorPoint(Point(0, 0));
    this->setPosition(position);
    this->addChild(sprite);
    this->schedule(schedule_selector(Tower::towerUpdate), 0.1f);
    status = STATUS_DONOTHING;
    
    healthBar = ProgressTimer::create(Sprite::create("health_bar_green.png"));
    healthBar->setType(ProgressTimer::Type::BAR);
    healthBar->setMidpoint(Point(0, 0));
    healthBar->setBarChangeRate(Point(1, 0));
    healthBar->setPercentage(100);
    healthBar->setScale(sprite->getContentSize().width / healthBar->getContentSize().width);
    healthBar->setAnchorPoint(Point(0,0));
    healthBar->setPosition(Point(0, -15));
    this->addChild(healthBar, 2);
    auto redbar = Sprite::create("health_bar_red.png");
    redbar->setPosition(healthBar->getPosition());
    redbar->setScale(sprite->getContentSize().width / redbar->getContentSize().width);
    redbar->setAnchorPoint(Point(0,0));
    this->addChild(redbar, 1);
    this->setDefaultProperty();
    GameScene::playerList->pushBack(this);
    type = TYPE_TOWER;
    return true;
}
void Tower::setDefaultProperty()
{
    pointWidthRate = 0.15f;
    pointHeightRate = 0.15f;
    widthRate = 0.7f;
    heightRate = 0.7f;
    runFrames = 0;
    attFrames = 0;
    type = TYPE_TOWER;
}
void Tower::setDamage(int damage, Entity* who)
{

    curHP -= damage;
    healthBar->setPercentage(float(curHP) / float(maxHP) * 100);
    if (curHP <= 0 && active)
    {
        active = false;
        setDead();
    }
}
void Tower::setDead()
{
    active = false;
    GameScene::playerList->eraseObject(this);
    //this->stopAllActions();
    auto dead1 = FadeOut::create(0.5f);
    auto dead2 = Animate::create(AnimationUtil::createAnimWithFrame("Effect_TowerExplosion", 0.1f, 1));
    auto tmp_sprite = Sprite::create();
    //CCLOG("%f, %f", getPosition().x, getPosition().y);
    tmp_sprite->setPosition(this->getMidPoint()-this->getPosition());
    this->addChild(tmp_sprite);
    tmp_sprite->runAction(dead2);
    sprite->runAction(Sequence::create(
                                         dead1,
                                         CallFunc::create(
                                                          [&](){removeSelf();}
                                                          ),
                                         NULL));
}
Entity* Tower::searchTarget()
{
    float min = BG_WIDTH * 2 + 1;
    Entity* attObject = NULL;
    for( Entity* enemy : *GameScene::enemyList)
    {
        if (enemy==NULL || enemy->hasRemoved) continue;
        if (enemy->getLine() != line || enemy->getHP()<=0)
            continue;
        float enemy_x = enemy->getPositionX();
        if (abs(enemy_x - this->getMidPoint().x)<=attRange && min>enemy_x)
        {
            min = enemy_x;
            attObject = enemy;
        }
    }
    return attObject;
}
void Tower::towerUpdate(float dt)
{
    
    if (status == STATUS_COOLDOWN)
        return;
    Entity* attObject = this->searchTarget();
    if (attObject==NULL && status==STATUS_ATTACKING)
    {
        status = STATUS_DONOTHING;
        this->doNothing();
        return;
    }
    if (attObject!=NULL)
    {
        this->animateAttack();
        this->attackObject(attObject);
        status = STATUS_COOLDOWN;
        this->runAction(Sequence::create(
                                         DelayTime::create(1.0f/attSpeed),
                                         CallFunc::create([&](){
                                                                status = STATUS_ATTACKING;
                                                                }),
                                         NULL
                                         ));
        
    }
}
void Tower::doNothing()
{
    this->stopAllActions();
}
void TowerMagic::animateAttack()
{
    Animate* animate = Animate::create(AnimationUtil::createAnimWithFrame(
                                                                          StringUtils::format("Effect_PurpleCircle"),
                                                                          1.0f/attSpeed/4,
                                                                          1)
                                       );
    auto att_sprite = Sprite::create();
    att_sprite->runAction(Sequence::create(animate,
                                          CallFunc::create([&](){this->removeChildByTag(11);}), NULL));
    att_sprite->setPosition(sprite->getContentSize().width/2, sprite->getContentSize().height);
    this->addChild(att_sprite, 2, 11);
}
void TowerMagic::attackObject(Entity *enemy)
{
    //CCLOG("attack!");
    
    Point tmp = this->getMidPoint();
    tmp.y += sprite->getContentSize().height / 2;
    BulletIntracing *bullet = BulletIntracing::create(enemy, attack,  tmp, "Bullet_PurpleBall", false, 0.6f, ENEMY_FACTION, 500, this);
    bullet->fire();
    this->getParent()->addChild(bullet, 3);
}
TowerMagic* TowerMagic::create(int line, float pX)
{
    TowerMagic* pRet = new TowerMagic;
    if (pRet && pRet->initWithProperty(MAGICTOWER_HP, MAGICTOWER_HP, MAGICTOWER_ATT, MAGICTOWER_ATTSPEED, MAGICTOWER_RANGE, pX, line, "Tower_Magic"))
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
TowerBarrack* TowerBarrack::create(int line, float pX)
{
    TowerBarrack* pRet = new TowerBarrack;
    if (pRet && pRet->initWithProperty(500, 500, 0, 0, 0, pX, line, "Tower_Barrack"))
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
void TowerBarrack::towerUpdate(float dt)
{
    int rd = rand()%100;
    if (rd<=20 || first)
    {
        first = false;
        auto soldierFox = SoldierFox::create(line, this->getMidPoint().x - 40);
        this->getParent()->addChild(soldierFox, 2);
    }
}
void TowerBarrack::setDefaultProperty()
{
    this->unscheduleAllSelectors();
    this->schedule(schedule_selector(TowerBarrack::towerUpdate), 1.0f);
    pointWidthRate = 0.15f;
    pointHeightRate = 0.15f;
    widthRate = 0.7f;
    heightRate = 0.7f;
    runFrames = 4;
    attFrames = 4;
    first = true;
}
void TowerRocket::animateAttack()
{
    Animate* animate = Animate::create(AnimationUtil::createAnimWithFrame(
                                                                          StringUtils::format("Effect_FireSmoke"),
                                                                          0.08f,
                                                                          1)
                                       );
    auto att_sprite = Sprite::create();
    att_sprite->runAction(Sequence::create(animate,
                                           CallFunc::create([&](){this->removeChildByTag(11);}), NULL));
    att_sprite->setPosition(sprite->getContentSize().width - 10, sprite->getContentSize().height - 10);
    att_sprite->setAnchorPoint(Point(0, 0.5));
    att_sprite->setRotation(315.0f);
    att_sprite->setScale(1.4f);
    this->addChild(att_sprite, 2, 11);
}
void TowerRocket::attackObject(Entity *enemy)
{
    //CCLOG("attack!");
    
    Point tmp = this->getMidPoint();
    tmp.y += sprite->getContentSize().height / 2;
    tmp.x += sprite->getContentSize().width / 2 ;
    int tmpAttack = attack;
    if (enemy->getType()==TYPE_FLY)
        tmpAttack*=2;
    BulletIntracing *bullet = BulletIntracing::create(enemy, tmpAttack,  tmp, "Bullet_Rocket", true, 0.5f, ENEMY_FACTION, 650, this);
    tmp.x += bullet->getContentSize().width / 2;
    bullet->fire();
    this->getParent()->addChild(bullet, 3);
}
TowerRocket* TowerRocket::create(int line, float pX)
{
    TowerRocket* pRet = new TowerRocket;
    if (pRet && pRet->initWithProperty(1500, 1500, 100, 0.5f, 900, pX, line, "Tower_Rocket"))
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
Entity* TowerRocket::searchTarget()
{
    float min = BG_WIDTH * 2 + 1;
    Entity* attObject = NULL;
    int targetType = TYPE_WALK;
    for( Entity* enemy : *GameScene::enemyList)
    {
        if (enemy==NULL || enemy->hasRemoved) continue;
        if (enemy->getLine() != line || enemy->getHP()<=0)
            continue;
        if (targetType == TYPE_FLY && enemy->getType()==TYPE_WALK)
            continue;
        float enemy_x = enemy->getPositionX();
        if (abs(enemy_x - this->getMidPoint().x)<=attRange && (min>enemy_x || (targetType==TYPE_WALK && enemy->getType()==TYPE_FLY)))
        {
            targetType = enemy->getType();
            min = enemy_x;
            attObject = enemy;
        }
    }
    return attObject;
}