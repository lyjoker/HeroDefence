//
//  MenuLayer.h
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#ifndef __HeroDeffence__MenuLayer__
#define __HeroDeffence__MenuLayer__
#include "cocos2d.h"
class GameScene;

class MenuLayer : public cocos2d::Layer
{
public:
    bool init();
    CREATE_FUNC(MenuLayer);
    cocos2d::LabelTTF* timeDisplayer, *goldDisplayer;
    void addGold(int _var);
    void addScore(int _var);
    void minusGold(int _var);
private:
    float nowTime;
    int nameIndex;
    void update(float dt);
    void pauseGame(cocos2d::Object* pSender);
    void resumeGame(cocos2d::Object* pSender);
    cocos2d::Scene* pauseScene(cocos2d::RenderTexture*);
    cocos2d::Layer* pauseLayer();
    cocos2d::Sprite* selSprite;
    CC_SYNTHESIZE(int, _gold, Gold);
    CC_SYNTHESIZE(int, _score, Score);
    std::vector<cocos2d::Sprite*> *towerIconList;
    std::string *nameList;
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    GameScene* getGameLayer();
    cocos2d::EventListenerTouchOneByOne* listener;
    void coolDown(int);
    void coolDownUpdate(float);
    
    
    //cocos2d::MenuItemImage* pauseItem;
};

#endif /* defined(__HeroDeffence__MenuLayer__) */
