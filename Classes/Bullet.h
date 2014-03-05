//
//  Bullet.h
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#ifndef __HeroDeffence__Bullet__
#define __HeroDeffence__Bullet__

#include "cocos2d.h"

class Entity;

class Bullet : public cocos2d::Node
{
public:
    virtual void removeSelf();
    virtual void fire()=0;
    virtual cocos2d::Rect getEffectRect();
    static float to360Angle(float angle)
	{
		if (angle > 360)
            angle = (int)angle % 360;
		if (angle < 0)
			angle += 360;
		return angle;
	}
    bool hasRemoved;
protected:
    bool active;
    int damage, speed, targetFaction;
    float scale;
    cocos2d::Point point;
    std::string name;
    cocos2d::Sprite* sprite;
};
class BulletIntracing : public Bullet
{
public:
    static BulletIntracing* create(Entity*, int, cocos2d::Point, const char* , bool, float, int,int, Entity*);
    bool initWithProperty(Entity*, int, cocos2d::Point, const char* , bool, float, int,int, Entity*);
    void fire();
    float calAngle();
protected:
    bool isExplosion;
    Entity* target;
    Entity* who;
    float angle;
    void update(float dt);
    void explosion();
};
#endif /* defined(__HeroDeffence__Bullet__) */
