//
//  MenuLayer.cpp
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#include "MenuLayer.h"
#include "Common.h"
#include "GameScene.h"

USING_NS_CC;
int magicTag = 11;
int rocketTag = 12;
int barrackTag = 13;

bool MenuLayer::init()
{
    timeDisplayer = LabelTTF::create("0.00", "Arial", 30);
    timeDisplayer->setPosition(Point(WIN_WIDTH - 50, WIN_HEIGHT - 50));
    this->addChild(timeDisplayer);
    nowTime = 0;
    
    goldDisplayer = LabelTTF::create(StringUtils::format("Gold: %i", getGold()), "Arial", 40);
    goldDisplayer->setColor(Color3B::YELLOW);
    
    goldDisplayer->setPosition(100, WIN_HEIGHT -50);
    this->addChild(goldDisplayer);
    
    
    this->scheduleUpdate();
    auto pauseItem = MenuItemImage::create("pause.png",
                                      "pause_selected.png",
                                      CC_CALLBACK_1(MenuLayer::pauseGame, this));
    pauseItem->setPosition(WIN_WIDTH - 60, WIN_HEIGHT -100);
    pauseItem->setScale(0.2f);
    auto menu = Menu::create(pauseItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
    
    
    auto iconMagic = Sprite::create("Icon_Magic.png");
    auto iconRocket = Sprite::create("Icon_Rocket.png");
    auto iconBarrack = Sprite::create("Icon_Barrack.png");
    iconMagic->setPosition(Point(TOWERICON_X1, TOWERICON_Y));
    iconRocket->setPosition(Point(TOWERICON_X2, TOWERICON_Y));
    iconBarrack->setPosition(Point(TOWERICON_X3, TOWERICON_Y));
    towerIconList = new std::vector<Sprite*>();
    towerIconList->push_back(iconMagic);
    towerIconList->push_back(iconRocket);
    towerIconList->push_back(iconBarrack);
    
    this->addChild(iconMagic, 1, magicTag);
    this->addChild(iconRocket, 1, rocketTag);
    this->addChild(iconBarrack, 1, barrackTag);
    
    listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(MenuLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MenuLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MenuLayer::onTouchEnded, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    nameList= new std::string[3];
    nameList[0] = "Tower_Magic";
    nameList[1] = "Tower_Rocket";
    nameList[2] = "Tower_Barrack";
    selSprite = NULL;
    
    setGold(1000);
    return true;
}
bool MenuLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    int i=0;
    for (Sprite* sprite : *towerIconList)
    {
        if (sprite->getBoundingBox().containsPoint(touch->getLocation()))
        {
            auto newSprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%s.png", nameList[i].c_str())));
            listener->setSwallowTouches(true);
            newSprite->setPosition(touch->getLocation());
            newSprite->setScale(this->getParent()->getChildByTag(GAMELAYER_TAG)->getScale());
            newSprite->setColor(Color3B::RED);
            newSprite->setOpacity(50);
            this->addChild(newSprite, 2, 111);
            selSprite = newSprite;
            nameIndex = i;
        }
        i++;
    }
    return true;
}
GameScene* MenuLayer::getGameLayer()
{
    return ((GameScene*)this->getParent()->getChildByTag(GAMELAYER_TAG));
}
void MenuLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (selSprite==NULL)
        return;
    selSprite->setPosition(touch->getLocation());
    if (((GameScene*)this->getParent()->getChildByTag(GAMELAYER_TAG))->canAddTower(touch->getLocation()))
    {
        selSprite->setColor(Color3B::GREEN);
        selSprite->setOpacity(200);
    }else
    {
        selSprite->setColor(Color3B::RED);
        selSprite->setOpacity(50);
    }
}
void MenuLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (selSprite==NULL)
        return;
    listener->setSwallowTouches(false);
    this->removeChildByTag(111);
    selSprite = NULL;
    if (((GameScene*)this->getParent()->getChildByTag(GAMELAYER_TAG))->canAddTower(touch->getLocation()))
    {
        if (((GameScene*)this->getParent()->getChildByTag(GAMELAYER_TAG))->addTower(nameList[nameIndex], touch->getLocation()))
        {
            coolDown(nameIndex);
            return;
        }
        else
        {
            CCLOG("add tower failed");
            return;
        }
    }
}
void MenuLayer::coolDown(int nameIndex)
{
    std::string name;
    switch (nameIndex) {
        case 0:
            name = "Icon_Magic_selected.png";
            break;
        case 1:
            name = "Icon_Rocket_selected.png";
            break;
        case 2:
            name = "Icon_Barrack_selected.png";
            break;
        default:
            break;
    }
    auto tmp = ProgressTimer::create(Sprite::create(name.c_str()));
    auto iconSprite = this->getChildByTag(nameIndex+11);
    tmp->setType(ProgressTimer::Type::RADIAL);
    tmp->setPercentage(1);
    iconSprite->addChild(tmp, 1, 1);
    auto tmp2 = LabelTTF::create();
    tmp2->setFontName("Arial");
    tmp2->setFontSize(10);
    iconSprite->addChild(tmp2, 2, 2);
    iconSprite->schedule(schedule_selector(MenuLayer::coolDownUpdate), 0.1f);
}
void MenuLayer::coolDownUpdate(float dt)
{
    
}
void MenuLayer::update(float dt)
{
    nowTime+=dt;
    timeDisplayer->setString(StringUtils::format("%.2f", nowTime));
    goldDisplayer->setString(StringUtils::format("Gold: %i", getGold()));
}
void MenuLayer::addGold(int _var)
{
    _gold+=_var;
}
void MenuLayer::minusGold(int _var)
{
    _gold-=_var;
}
void MenuLayer::addScore(int _var)
{
    _score+=_var;
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