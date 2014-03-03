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
    nowTime = 0;
    this->scheduleUpdate();
    auto pauseItem = MenuItemImage::create("pause.png",
                                      "pause_selected.png",
                                      CC_CALLBACK_1(MenuLayer::pauseGame, this));
    pauseItem->setPosition(WIN_WIDTH - 60, WIN_HEIGHT -100);
    pauseItem->setScale(0.2f);
    auto menu = Menu::create(pauseItem, NULL);
    menu->setPosition(Point::ZERO);
    //menu->setScale(0.2f);
    this->addChild(menu);
    return true;
}
void MenuLayer::update(float dt)
{
    nowTime+=dt;
    timeDisplayer->setString(StringUtils::format("%.2f", nowTime));
}
void MenuLayer::pauseGame(Object* pSender)
{
    auto render = RenderTexture::create(WIN_WIDTH, WIN_HEIGHT);
    render->begin();
    this->getParent()->visit();
    render->end();
    Director::getInstance()->pushScene(pauseScene(render));

}
Scene* MenuLayer::pauseScene(RenderTexture* pRender)
{
    auto pauseScene = Scene::create();
    auto bg = Sprite::createWithTexture(pRender->getSprite()->getTexture());
    bg->setPosition(Point(WIN_WIDTH/2, WIN_HEIGHT/2));
    //bg->setOpacity(128);
    bg->setColor(Color3B::GRAY);
    bg->setFlippedY(true);
    pauseScene->addChild(bg);
    pauseScene->addChild(pauseLayer());
    return pauseScene;
}
Layer* MenuLayer::pauseLayer()
{
    auto pauseLayer = Layer::create();
    //auto label = Label::create();
    auto resumeItem = MenuItemLabel::create(LabelTTF::create("Resume", "Arial", 60),
                                            CC_CALLBACK_1(MenuLayer::resumeGame, this));
    resumeItem->setPosition(Point(WIN_WIDTH/2, WIN_HEIGHT/2));
    auto menu = Menu::create(resumeItem, NULL);
    menu->setPosition(Point::ZERO);
    pauseLayer->addChild(menu);
    return pauseLayer;
}

void MenuLayer::resumeGame(Object* pSender)
{
    Director::getInstance()->popScene();
}