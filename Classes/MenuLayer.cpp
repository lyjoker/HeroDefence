//
//  MenuLayer.cpp
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#include "MenuLayer.h"
#include "Common.h"

USING_NS_CC;


bool MenuLayer::init()
{
    timeDisplayer = LabelTTF::create("0.00", "Arial", 30);
    timeDisplayer->setPosition(Point(WIN_WIDTH - 50, WIN_HEIGHT - 50));
    this->addChild(timeDisplayer);
    return true;
}