//
//  Hero.h
//  HeroDeffence
//
//  Created by Yuan Lin on 3/5/14.
//
//

#ifndef __HeroDeffence__Hero__
#define __HeroDeffence__Hero__

#include "cocos2d.h"
#include "Entity.h"

class Hero : public Entity
{
public:
    bool initWithProperty(const char* pName, int pHP, int pSpeed, int pAttack, int pLine, float pX, float pAttSpeed, int pAttRange);
    virtual void setDamage(int damage, Entity* who);
    virtual void runToDest(cocos2d::Point _dest);
protected:
    virtual void heroUpdate(float dt);
    virtual void animateRun();
    virtual void animateAttack();
    virtual void animateWait();
    virtual void dead();
    virtual void setDead();
    virtual void attackObject(Entity* target);
    virtual void reborn(float);
    
    bool direction;
    cocos2d::Point dest;
};

class HeroCat : public Hero
{
public:
    static HeroCat* create();
private:
    void setDefaultProperty();
    void attackObject(Entity* target);
};
#endif /* defined(__HeroDeffence__Hero__) */
