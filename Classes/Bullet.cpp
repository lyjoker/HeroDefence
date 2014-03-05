//
//  Bullet.cpp
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#include "Bullet.h"
#include "Common.h"
#include "Entity.h"
#include "GameScene.h"
#include "AnimationUtil.h"

USING_NS_CC;

void Bullet::removeSelf()
{
    if(hasRemoved)
		return;
    
	hasRemoved = true;
	this->unscheduleAllSelectors();
	this->stopAllActions();
	this->removeFromParentAndCleanup(true);
}
Rect Bullet::getEffectRect()
{
    float _width = sprite->getContentSize().width;
    float _height = sprite->getContentSize().height;
    auto rect = Rect(
                     this->getPositionX() - _width * 0.35,
                     this->getPositionY() - _height * 0.35,
                     _width * 0.7,
                     _height * 0.7
                     );
    return rect;
}

bool BulletIntracing::initWithProperty(Entity* _target, int _damage, Point _point, const char* _name, bool _isExplosion, float _scale, int _targetFaction , int _speed, Entity* _who)
{
    target = _target;
    damage = _damage;
    point = _point;
    name = _name;
    isExplosion = _isExplosion;
    speed = _speed;
    scale = _scale;
    targetFaction = _targetFaction;
    who = _who;
    scheduleUpdate();
    return true;
}
BulletIntracing* BulletIntracing::create(Entity* _target, int _damage, Point _point, const char* _name, bool _isExplosion, float _scale, int _targetFaction,int _speed, Entity* _who)
{
    BulletIntracing* pRet = new BulletIntracing();
    if (pRet && pRet->initWithProperty(_target, _damage, _point, _name, _isExplosion, _scale, _targetFaction,_speed, _who))
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
float BulletIntracing::calAngle()
{
    float dx = target->getMidPoint().x - point.x;
    float dy = target->getMidPoint().y - point.y;
    float tanangle = dy / dx;
    angle = atanf(tanangle);
    angle = to360Angle(CC_RADIANS_TO_DEGREES(angle));
    if (dx<0 && dy>0)
        angle -= 180;
    else if (dx<0 && dy<0)
        angle += 180;
    return angle;
}
void BulletIntracing::fire()
{
    if (target==NULL)
        return;
    auto frameCache = SpriteFrameCache::getInstance();
    SpriteFrame* tmp = frameCache->getSpriteFrameByName(StringUtils::format("%s.png", name.c_str()));
    sprite = Sprite::createWithSpriteFrame(tmp);
    sprite->setScale(scale);
    this->addChild(sprite);
    this->setPosition(point);
    angle = calAngle();
    this->setRotation(360-angle);
    this->setAnchorPoint(Point(0.5, 0.5));
    active = true;
}
void BulletIntracing::explosion()
{
    active = false;
    auto frameCache = SpriteFrameCache::getInstance();
    SpriteFrame* tmp = frameCache->getSpriteFrameByName(StringUtils::format("%s_Exp.png", name.c_str()));
    sprite->removeFromParentAndCleanup(true);
    FiniteTimeAction* tmpAnimate = NULL;
    if (tmp!=NULL)
        sprite = Sprite::createWithSpriteFrame(tmp);
    else
    {
        sprite = Sprite::createWithSpriteFrame(frameCache->getSpriteFrameByName(StringUtils::format("%s_Exp1.png", name.c_str())));
        //tmpAnimate = Animate::create(AnimationUtil::createAnimWithFrame("%s_Exp", 0.1f, 1));
        sprite->setScale(1.5f);
    }
    this->addChild(sprite);
    Vector<Entity*> *list = NULL;
    if (targetFaction == ENEMY_FACTION)
        list = GameScene::enemyList;
    else if (targetFaction == PLAYER_FACTION)
        list = GameScene::playerList;
    if (list == NULL) return;
    float _width = sprite->getContentSize().width;
    float _height = sprite->getContentSize().height;
    Rect tRect = Rect(this->getPositionX() - _width/2 , this->getPositionY()-_height/2, _width, _height);
    //CCLOG("%f, %f", tRect.getMinX(), tRect.getMinY());
    for (Entity* object : *list)
    {
        if (object->hasRemoved || object->getHP()<=0)
            continue;
        if (object->getEffectRect().intersectsRect(tRect))
        {
            object->setDamage(damage, who);
        }
    }
    if (tmpAnimate == NULL)
        tmpAnimate = FadeOut::create(0.5f);
    tmpAnimate = Animate::create(AnimationUtil::createAnimWithFrame(StringUtils::format("%s_Exp", name.c_str()), 0.07f, 1));
    sprite->runAction(Sequence::create(tmpAnimate,
                                       CallFunc::create(CC_CALLBACK_0(Bullet::removeSelf, this)),
                                       NULL));
    //CCLOG("point: %f, %f", point.x, point.y);
    
    
}
void BulletIntracing::update(float dt)
{
    if (active)
    {
        if (target==NULL || target->hasRemoved || target->getHP()<=0)
        {
            removeSelf();
            return;
        }
        if (this->getEffectRect().containsPoint(target->getMidPoint()))
        {
            
            //target->setDamage(damage);
            if (isExplosion)
                explosion();
            else
            {
                target->setDamage(damage, who);
                removeSelf();
            }
            return;
        }
        angle = calAngle();
        float radian = CC_DEGREES_TO_RADIANS(angle);
        point.x += cosf(radian) * speed * dt;
        point.y += sinf(radian) * speed * dt;
        this->setPosition(point);
        this->setRotation(360 - angle);
    }
}

