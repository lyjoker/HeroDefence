//
//  Soldier.h
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#ifndef __HeroDeffence__Soldier__
#define __HeroDeffence__Soldier__

#include "cocos2d.h"
#include "Entity.h"

class Soldier : public Entity
{
public:
    bool initWithProperty(const char* pName, int pHP, int pSpeed, int pAttack, int pLine, float pX, float pAttSpeed, int pAttRange);
    virtual void setDamage(int damage);
protected:
    virtual void soldierUpdate(float dt);
    virtual void animateRun();
    virtual void animateAttack();
    virtual void setDead();
    virtual void attackObject(Entity* target);
    virtual void runToDest();
    cocos2d::Point endPoint;
    bool direction;
};

class SoldierFox : public Soldier
{
public:
    static SoldierFox* create(int pLine, float pX);
private:
    void setDefaultProperty();
};
#endif /* defined(__HeroDeffence__Soldier__) */
