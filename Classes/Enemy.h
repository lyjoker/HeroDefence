//
//  Enemy.h
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#ifndef __HeroDeffence__Enemy__
#define __HeroDeffence__Enemy__

#include "Entity.h"

class Enemy : public Entity
{
public:
    bool initWithProperty(const char* pName, int pHP, int pSpeed, int pAttack, int pLine, float pX, float pAttSpeed, int pAttRange);
    virtual void setDamage(int damage);
protected:
    virtual void enemyUpdate(float dt);
    virtual void animateRun();
    virtual void animateAttack();
    virtual void setDead();
    virtual void attackObject(Entity* target);
    virtual void runToDest();
    cocos2d::Point endPoint;
};
class EnemyKnight : public Enemy
{
public:
    static EnemyKnight* create(int pLine, float pX);
};

class EnemyBlueDragon : public Enemy
{
public:
    static EnemyBlueDragon* create(int pLine, float pX);
    void setDefaultProperty();
    void attackObject(Entity* target);
    void animateAttack();
};
#endif /* defined(__HeroDeffence__Enemy__) */
