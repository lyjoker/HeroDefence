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

class MenuLayer : public cocos2d::Layer
{
public:
    bool init();
    CREATE_FUNC(MenuLayer);
    cocos2d::LabelTTF* timeDisplayer;
private:
    float nowTime;
    void update(float dt);
    void pauseGame(cocos2d::Object* pSender);
    void resumeGame(cocos2d::Object* pSender);
    cocos2d::Scene* pauseScene(cocos2d::RenderTexture*);
    cocos2d::Layer* pauseLayer();
    //cocos2d::MenuItemImage* pauseItem;
};

#endif /* defined(__HeroDeffence__MenuLayer__) */
