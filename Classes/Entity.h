//
//  Entity.h
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#ifndef __HeroDeffence__Entity__
#define __HeroDeffence__Entity__

#include "cocos2d.h"
#include "Common.h"


class Entity : public cocos2d::Node
{
public:
    cocos2d::Point getMidPoint();
    cocos2d::Rect getEffectRect();
    cocos2d::Rect getBoundingBox();
    int getLine();
    int getHP();
    int getType();
    bool hasRemoved;
    virtual void setDamage(int, Entity*) = 0;
protected:
    int curHP, maxHP, attack, speed, status, line, attRange;
    float attSpeed;
    float pointWidthRate, pointHeightRate, widthRate, heightRate;
    int runFrames, attFrames;
    std::string name;
    cocos2d::Sprite* sprite;
    cocos2d::ProgressTimer* healthBar;
    bool active;
    void removeSelf();
    int type;
    virtual void setDefaultProperty();
    virtual void setDead() = 0;
};
#endif /* defined(__HeroDeffence__Entity__) */
