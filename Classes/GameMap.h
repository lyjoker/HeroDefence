//
//  GameMap.h
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#ifndef __HeroDeffence__GameMap__
#define __HeroDeffence__GameMap__

#include "cocos2d.h"


class GameMap
{
public:
    static cocos2d::Point MapPoint(int line, float position, float nowScale);
    static float lineToY(int line);
    
};
#endif /* defined(__HeroDeffence__GameMap__) */
