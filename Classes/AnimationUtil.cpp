//
//  AnimationUtil.cpp
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#include "AnimationUtil.h"

USING_NS_CC;

Animation* AnimationUtil::createAnimWithFrame(std::string name, float delay, int iLoops)
{
    auto cache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> framesArray = Vector<SpriteFrame*>();
    SpriteFrame* frame = NULL;
    int index = 1;
    do {
        std::string tmp = StringUtils::format("%s%d.png", name.c_str(), index++);
        frame = cache->getSpriteFrameByName(tmp);
        if (frame == NULL)
            break;
        framesArray.pushBack(frame);
    } while (true);
    Animation* animation = Animation::createWithSpriteFrames(framesArray);
    animation->setLoops(iLoops);
    animation->setRestoreOriginalFrame(true);
    animation->setDelayPerUnit(delay);
    return animation;
    
}

