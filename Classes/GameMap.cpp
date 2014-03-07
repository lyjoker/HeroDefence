//
//  GameMap.cpp
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#include "GameMap.h"
#include "Common.h"

USING_NS_CC;




Point GameMap::MapPoint(int line, float position, float nowScale)
{
    if (line == 1)
        return Point(position*nowScale, LINE1);
    if (line == 2)
        return Point(position*nowScale, LINE2);
    return Point(position*nowScale, LINE1);
}
float GameMap::lineToY(int line)
{
    if (line == 1)
        return LINE1;
    else if (line == 2)
        return LINE2;
    else if (line == 3)
        return LINE3;
    return LINE2;
}
float GameMap::calAngle(const cocos2d::Point &p1, const cocos2d::Point &p2)
{
    float angle;
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float tanangle = dy / dx;
    angle = atanf(tanangle);
    angle = CC_RADIANS_TO_DEGREES(angle);
    if (angle > 360)
        angle = (int)angle % 360;
    if (angle < 0)
        angle += 360;
    if (dx<0 && dy>0)
        angle -= 180;
    else if (dx<0 && dy<0)
        angle += 180;
    return angle;
}