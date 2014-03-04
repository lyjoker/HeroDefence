//
//  Entity.cpp
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#include "Entity.h"

USING_NS_CC;

int Entity::getLine()
{
    return line;
}
int Entity::getHP()
{
    return curHP;
}
int Entity::getType()
{
    return type;
}
void Entity::setDefaultProperty()
{
    pointWidthRate = 0.15f;
    pointHeightRate = 0.15f;
    widthRate = 0.7f;
    heightRate = 0.7f;
    runFrames = 4;
    attFrames = 4;
    type = TYPE_WALK;
}
Rect Entity::getBoundingBox()
{
    return Rect(this->getPositionX(), this->getPositionY(), sprite->getContentSize().width, sprite->getContentSize().height);
}
Rect Entity::getEffectRect()
{
    float _width = sprite->getContentSize().width;
    float _height = sprite->getContentSize().height;
    auto rect = Rect(
                     this->getPositionX() + _width * pointWidthRate,
                     this->getPositionY() + _height * pointHeightRate,
                     _width * widthRate,
                     _height * heightRate
                     );
    return rect;
}
Point Entity::getMidPoint()
{
    return (Point(
                  this->getPositionX() + sprite->getContentSize().width / 2,
                  this->getPositionY() + sprite->getContentSize().height / 2
                  )
            );
}
void Entity::removeSelf()
{
    if(hasRemoved)
		return;
    
	hasRemoved = true;
	unscheduleAllSelectors();
	this->stopAllActions();
    sprite->stopAllActions();
	this->removeFromParentAndCleanup(true);
    
}