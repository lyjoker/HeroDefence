//
//  IconSprite.cpp
//  HeroDeffence
//
//  Created by Yuan Lin on 3/5/14.
//
//

#include "IconSprite.h"

USING_NS_CC;

IconSprite* IconSprite::create(const char* _name, int _cdTime)
{
    IconSprite* pRet = new IconSprite;
    if (pRet && pRet->initWithName(_name, _cdTime))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool IconSprite::initWithName(const char* _name, int _cdTime)
{
    name = _name;
    cdTime = _cdTime;
    nowcdTime = 0;
    sprite = Sprite::create(StringUtils::format("Icon_%s.png", name.c_str()));
    disableSprite = Sprite::create(StringUtils::format("Icon_%s_selected.png", name.c_str()));
    disableSprite->setColor(Color3B::RED);
    this->addChild(sprite, 1);
    this->addChild(disableSprite, 2);
    disableSprite->setVisible(false);
    _isEnable = true;
    _isCoolDown = false;
    return true;
}
void IconSprite::setEnable(bool _enable)
{
    sprite->setVisible(_enable);
    disableSprite->setVisible(!_enable);
    _isEnable = _enable;
}
Rect IconSprite::getBoundingBox()
{
    float _width = sprite->getContentSize().width * this->getScaleX();
    float _height = sprite->getContentSize().height * this->getScaleY();
    return Rect(this->getPositionX() - _width / 2,
                this->getPositionY() - _height / 2,
                _width,
                _height);
}
bool IconSprite::setCoolDown()
{
    if (_isCoolDown)
        return false;
    _isCoolDown = true;
    auto tmpSprite = Sprite::create(StringUtils::format("Icon_%s_selected.png", name.c_str()));
    Color3B tmpColor = tmpSprite->getColor();
    tmpColor.r /= 2;
    tmpColor.g /= 2;
    tmpColor.b /= 2;
    tmpSprite->setColor(tmpColor);
    cdImage = ProgressTimer::create(tmpSprite);
    cdImage->setType(ProgressTimer::Type::RADIAL);
    cdImage->setPercentage(100);
    cdImage->setReverseProgress(true);
    cdLabel = LabelTTF::create();
    cdLabel->setFontName("Arial");
    cdLabel->setFontSize(20);
    this->addChild(cdImage, 3);
    this->addChild(cdLabel, 4);
    this->schedule(schedule_selector(IconSprite::update), 0.1f);
    nowcdTime = cdTime;
    return true;
}
void IconSprite::update(float dt)
{
    if (nowcdTime>=10)
        cdLabel->setString(StringUtils::format("%.0f", nowcdTime));
    else
        cdLabel->setString(StringUtils::format("%.1f", nowcdTime));
    cdImage->setPercentage(nowcdTime/cdTime*100.0f);
    nowcdTime-=dt;
    if (nowcdTime<=0)
    {
        this->unscheduleAllSelectors();
        nowcdTime = 0;
        cdImage->removeFromParentAndCleanup(true);
        cdLabel->removeFromParentAndCleanup(true);
        _isCoolDown = false;
    }
    
}