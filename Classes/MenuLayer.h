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
};

#endif /* defined(__HeroDeffence__MenuLayer__) */
