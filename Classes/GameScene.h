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
    static ZoomScrollView* sLayer;
    bool init();
    void setTimeDisplay(cocos2d::LabelTTF *);
    CREATE_FUNC(GameScene);
    
private:
    void initBG();
    cocos2d::Sprite* m_bgSprite;
    void update(float dt);
    float scaleNow;
    bool test;
    //bool onTouchBGBegan(cocos2d::Touch*, cocos2d::Event*);
    //void onTouchBGMoved(cocos2d::Touch*, cocos2d::Event*);
    //void onTouchBGEnded(cocos2d::Touch*, cocos2d::Event*);
    //cocos2d::Point nowTouchPoint;
    //void updateEdges();
    //float maxWidth, minWidth, maxHeight, minHeight;
    void initFrameCache();
    
};



#endif /* defined(__HeroDeffence__GameScene__) */
