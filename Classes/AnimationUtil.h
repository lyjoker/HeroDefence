//
//  AnimationUtil.h
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#ifndef __HeroDeffence__AnimationUtil__
#define __HeroDeffence__AnimationUtil__

#include "cocos2d.h"

class AnimationUtil{
public:
    static cocos2d::Animation* createAnimWithFrame(std::string name, float delay, int iLoops);
    
};

#endif /* defined(__HeroDeffence__AnimationUtil__) */
