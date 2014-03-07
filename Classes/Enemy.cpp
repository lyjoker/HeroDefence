//
//  Enemy.cpp
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#include "Enemy.h"
#include "GameMap.h"
#include "AnimationUtil.h"
#include "GameScene.h"
#include "MenuLayer.h"
#include "Bullet.h"

USING_NS_CC;

bool Enemy::initWithProperty(const char* pName, int pHP, int pSpeed, int pAttack, int pLine, float pX, float pAttSpeed, int pAttRange, int pGold)
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
    setReward(pGold);
    active = true;
    nowPx = pX;
    auto frameCache = SpriteFrameCache::getInstance();
    sprite = Sprite::createWithSpriteFrame(
                                             frameCache->getSpriteFrameByName(
                                                                              StringUtils::format("%s_run1.png", name.c_str())
                                                                              )
                                             );
    sprite->setAnchorPoint(Point(0.5, 0));
    sprite->setFlippedX(true);
    this->setPosition(position);
    this->addChild(sprite);
    status = STATUS_RUNNING;
    endPoint = position;
    endPoint.x = 0;
    
    healthBar = ProgressTimer::create(Sprite::create("health_bar_green.png"));
    healthBar->setType(ProgressTimer::Type::BAR);
    healthBar->setMidpoint(Point(0, 0));
    healthBar->setBarChangeRate(Point(1, 0));
    healthBar->setPercentage(100);
    healthBar->setScale(sprite->getContentSize().width / healthBar->getContentSize().width);
    healthBar->setAnchorPoint(Point(0.5,0));
    healthBar->setPosition(Point(0, sprite->getContentSize().height));
    this->addChild(healthBar, 2);
    auto redbar = Sprite::create("health_bar_red.png");
    redbar->setPosition(healthBar->getPosition());
    redbar->setScale(sprite->getContentSize().width / redbar->getContentSize().width);
    redbar->setAnchorPoint(Point(0.5,0));
    this->addChild(redbar, 1);
    
    schedule(schedule_selector(Enemy::enemyUpdate), 0.1f);
    
    this->setDefaultProperty();
    
    
    /*Point tmp = m_position;
     float dist = tmp.x;
     tmp.x = 0;
     auto moveto = MoveTo::create(dist/m_speed, tmp);
     
     
     this->animateRun();
     this->runAction(moveto);
     */
    GameScene::enemyList->pushBack(this);
    this->runToDest();
    /*
     auto enemyListener = EventListenerTouchOneByOne::create();
     enemyListener->onTouchBegan = [=](Touch* touch, Event* event)->bool{
     
     Point tmp = this->convertToNodeSpace(touch->getLocation());
     
     
     if (m_Sprite->boundingBox().containsPoint(tmp))
     {
     if (this->m_status == STATUS_RUNNING)
     {
     this->attack();
     }else if (this->m_status == STATUS_ATTACKING)
     {
     this->run();
     }
     }
     return true;
     };
     getEventDispatcher()->addEventListenerWithSceneGraphPriority(enemyListener, m_Sprite);
     */
    return true;
}
void Enemy::setDamage(int damage, Entity* who)
{
    curHP -= damage;
    healthBar->setPercentage(float(curHP) / float(maxHP) * 100);
    if (curHP <= 0 && active)
    {
        if (who->getType()!=TYPE_HERO)
            GameScene::menulayer->addGold(getReward());
        else
            GameScene::menulayer->addGold(getReward()*1.3f);
        setDead();
    }
}
void Enemy::setDead()
{
    active = false;
    GameScene::enemyList->eraseObject(this);
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

void Enemy::animateRun()
{
    
    Animate* animate = Animate::create(AnimationUtil::createAnimWithFrame(StringUtils::format("%s_run", name.c_str()), 60.0f/runFrames/speed, -1));
    if (animate == NULL) return;
    sprite->stopAllActions();
    sprite->runAction(animate);
}
void Enemy::animateAttack()
{
    
    Animate* animate = Animate::create(AnimationUtil::createAnimWithFrame(StringUtils::format("%s_attack", name.c_str()), 0.7f/attFrames/attSpeed, 1));
    if (animate == NULL) return;
    sprite->stopAllActions();
    sprite->runAction(animate);
    
}
void Enemy::attackObject(Entity* target)
{
    if (target==NULL || target->hasRemoved || target->getHP()<=0)
        return;
    target->setDamage(attack, this);
}
void Enemy::enemyUpdate(float dt)
{
    if (status == STATUS_COOLDOWN || !active || hasRemoved)
        return;
    Entity* attObject = NULL;
    for( Entity* object : *GameScene::playerList)
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
    if (attObject==NULL && status==STATUS_RUNNING)
    {
        if (this->getPositionX()==nowPx)
            this->runToDest();
        nowPx = this->getPositionX();
    }
    if (attObject!=NULL)
    {
        this->stopAllActions();
        this->animateAttack();
        
        status = STATUS_COOLDOWN;
        this->runAction(Sequence::create(
                                         DelayTime::create(0.4f/attSpeed),
                                         CallFunc::create([=](){attackObject(attObject);}),
                                         DelayTime::create(0.6f/attSpeed),
                                         CallFunc::create([&](){status = STATUS_ATTACKING;}),
                                         NULL
                                         ));
        
    }
    if (this->getPositionX()<=endPoint.x+1)
    {
        //EventCustom event("Game Over");
        //_eventDispatcher->dispatchEvent(&event);
        removeSelf();
    }
    
}
void Enemy::runToDest()
{
    float dist = abs(this->getPosition().x - endPoint.x);
    auto moveto = MoveTo::create(dist/speed, endPoint);
    this->animateRun();
    this->runAction(moveto);
}

EnemyKnight* EnemyKnight::create(int pLine, float pX)
{
    EnemyKnight *pRet = new EnemyKnight();
    if (pRet && pRet->initWithProperty("Enemy_Knight", 1500, 60, 40, pLine, pX, 1.0f, 0, 40))
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
EnemyBlueDragon* EnemyBlueDragon::create(int pLine, float pX)
{
    EnemyBlueDragon *pRet = new EnemyBlueDragon();
    if (pRet && pRet->initWithProperty("Enemy_BlueDragon", 800, 100, 60, pLine, pX, 0.8f, 350, 60))
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

void EnemyBlueDragon::setDefaultProperty()
{
    pointWidthRate = 0.1f;
    pointHeightRate = 0.4f;
    widthRate = 0.5f;
    heightRate = 0.3f;
    this->setPosition(this->getPosition()+Point(0, 250+rand()%80-40));
    endPoint = this->getPosition();
    endPoint.x = 0;
    runFrames = 8;
    type = TYPE_FLY;
    //schedule(schedule_selector(EnemyBlueDragon::enemyUpdate), 0.1f);
}
void EnemyBlueDragon::attackObject(Entity* target)
{
    if (target==NULL || target->hasRemoved || target->getHP()<=0)
        return;
    Point tmp = this->getMidPoint();
    tmp.x -= sprite->getContentSize().width / 2;
    BulletIntracing *bullet = BulletIntracing::create(target, attack,  tmp, "Bullet_SmallFireBall", false, 0.8f, PLAYER_FACTION, 550, this);
    bullet->fire();
    this->getParent()->addChild(bullet, 3);
}
void EnemyBlueDragon::animateAttack()
{
    this->animateRun();
}
