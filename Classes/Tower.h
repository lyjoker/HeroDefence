//
//  Tower.h
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#ifndef __HeroDeffence__Tower__
#define __HeroDeffence__Tower__

#include "cocos2d.h"
#include "Entity.h"

class Tower : public Entity
{
public:
    bool initWithProperty(int, int,int, float,int, float,int, std::string);
    virtual void setDamage(int damage);
    
protected:
    virtual void setDead();
    virtual void towerUpdate(float dt);
    virtual void animateAttack(){};
    virtual void attackObject(Entity* target){};
    virtual void doNothing();
    virtual void setDefaultProperty();
    virtual Entity* searchTarget();
};

class TowerMagic : public Tower
{
public:
    static TowerMagic* create(int _line, float _pX);
private:
    void attackObject(Entity* target);
    void animateAttack();
};
class TowerBarrack : public Tower
{
public:
    static TowerBarrack* create(int _line, float _pX);
private:
    void towerUpdate(float dt);
    void setDefaultProperty();
    bool first;
};
class TowerRocket : public Tower
{
public:
    static TowerRocket* create(int _line, float _pX);
private:
    void attackObject(Entity* target);
    void animateAttack();
    Entity* searchTarget();
};
#endif /* defined(__HeroDeffence__Tower__) */
