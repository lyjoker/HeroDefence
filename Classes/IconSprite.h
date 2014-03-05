//
//  IconSprite.h
//  HeroDeffence
//
//  Created by Yuan Lin on 3/5/14.
//
//

#ifndef __HeroDeffence__IconSprite__
#define __HeroDeffence__IconSprite__

#include "cocos2d.h"

class IconSprite : public cocos2d::Node
{
public:
    static IconSprite* create(const char* _name, int _cdTime);
    cocos2d::Rect getBoundingBox();
    bool setCoolDown();
    void setEnable(bool);
    const char* getName(){return name.c_str();};
protected:
    void update(float);
    bool initWithName(const char* _name, int _cdTime);
    cocos2d::Sprite* sprite, *disableSprite;
    std::string name;
    float cdTime, nowcdTime;
    cocos2d::ProgressTimer *cdImage;
    cocos2d::LabelTTF *cdLabel;
    CC_SYNTHESIZE_READONLY(bool, _isEnable, isEnable);
    CC_SYNTHESIZE_READONLY(bool, _isCoolDown, isCoolDown);
};

#endif /* defined(__HeroDeffence__IconSprite__) */
