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

class MenuLayer;
class Entity;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    static MenuLayer* menulayer;
    static cocos2d::Vector<Entity*> *enemyList;
    static cocos2d::Vector<Entity*> *playerList;
    bool init();
    void setTimeDisplay(cocos2d::LabelTTF *);
    CREATE_FUNC(GameScene);
    
private:
    void initBG();
    cocos2d::Sprite* m_bgSprite;
    void update(float dt);
    float scaleNow, nowTime;
    bool onTouchBGBegan(cocos2d::Touch*, cocos2d::Event*);
    void onTouchBGMoved(cocos2d::Touch*, cocos2d::Event*);
    void onTouchBGEnded(cocos2d::Touch*, cocos2d::Event*);
    cocos2d::Point nowTouchPoint;
    void updateEdges();
    float maxWidth, minWidth, maxHeight, minHeight;
    void initFrameCache();
    
};


#endif /* defined(__HeroDeffence__GameScene__) */
