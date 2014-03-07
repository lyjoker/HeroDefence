//
//  GameScene.h
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#ifndef __HeroDeffence__GameScene__
#define __HeroDeffence__GameScene__

#include "cocos2d.h"
#include "cocos-ext.h"

class MenuLayer;
class Entity;
class Hero;


class ZoomScrollView : public cocos2d::extension::ScrollView
{
public:
    static ZoomScrollView* create(cocos2d::Size size, cocos2d::Node* container=NULL)
    {
        ZoomScrollView* pRet = new ZoomScrollView();
        if (pRet && pRet->initWithViewSize(size, container))
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
    void setMinScale(int);
};

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    static MenuLayer* menulayer;
    static cocos2d::Vector<Entity*> *enemyList;
    static cocos2d::Vector<Entity*> *playerList;
    Hero* hero;
    //static ZoomScrollView* sLayer;
    bool init();
    void setTimeDisplay(cocos2d::LabelTTF *);
    bool canAddTower(cocos2d::Point touchPoint);
    bool addTower(std::string towerName, cocos2d::Point location);
    void focusOnHero();
    bool heroMoveUp();
    bool heroMoveDown();
    void heroDead();
    bool heroSkillFirst();
    bool heroSkillSecond();
    void setEnableMove(bool);
    CREATE_FUNC(GameScene);
    
private:
    void initBG();
    cocos2d::Sprite* m_bgSprite;
    void update(float dt);
    float scaleNow;
    bool test, enableMove;
    //bool onTouchBGBegan(cocos2d::Touch*, cocos2d::Event*);
    //void onTouchBGMoved(cocos2d::Touch*, cocos2d::Event*);
    //void onTouchBGEnded(cocos2d::Touch*, cocos2d::Event*);
    void onTouchesBGBegan(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event*);
    void onTouchesBGMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event*);
    void onTouchesBGEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event*);
    cocos2d::Point nowTouchPoint, beginPoint;
    void updateEdges();
    float maxWidth, minWidth, maxHeight, minHeight;
    float maxScale, minScale;
    void initFrameCache();
    cocos2d::Point tuningPoint(cocos2d::Point);
    
};



#endif /* defined(__HeroDeffence__GameScene__) */
