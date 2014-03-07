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

class GameScene;

class Hero : public Entity
{
public:
    bool initWithProperty(const char* pName, int pHP, int pSpeed, int pAttack, int pLine, float pX, float pAttSpeed, int pAttRange);
    virtual void setDamage(int damage, Entity* who);
    virtual void runToDest(cocos2d::Point _dest);
    virtual bool moveUp();
    virtual bool moveDown();
    virtual void skillFirst()=0;
    virtual void skillSecond()=0;
    virtual void interrupt();
protected:
    virtual void heroUpdate(float dt);
    virtual void animateRun();
    virtual void animateAttack();
    virtual void animateWait();
    virtual void dead();
    virtual void setDead();
    virtual void attackObject(Entity* target);
    virtual void reborn(float);
    GameScene* getGameLayer();
    
    bool direction;
    cocos2d::Point dest;
    cocos2d::EventListenerTouchOneByOne* listener;
    bool listenerOn;
};

class HeroCat : public Hero
{
public:
    static HeroCat* create();
    void skillFirst();
    void skillSecond();
    void interrupt();
private:
    void setDefaultProperty();
    void attackObject(Entity* target);
    void skillFirstAttack();
    void skillSecondAttack(float);
    void skillSecondReady(float);
    
};
#endif /* defined(__HeroDeffence__Hero__) */
