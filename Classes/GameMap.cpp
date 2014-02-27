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