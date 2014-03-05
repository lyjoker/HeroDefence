//
//  GameScene.cpp
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#include "GameScene.h"
#include "GameMap.h"
#include "Common.h"
#include "Entity.h"
#include "MenuLayer.h"
#include "Tower.h"
#include "Enemy.h"
#include "Hero.h"


USING_NS_CC;
USING_NS_CC_EXT;

//Size visibleSize = Director::getInstance()->getVisibleSize();
float MAP_POS_X = WIN_WIDTH / 2;
float MAP_POS_Y = WIN_HEIGHT / 2;

float touchDist = 0;
Point midPoint;
bool moved;

MenuLayer* GameScene::menulayer = NULL;
//ZoomScrollView* GameScene::sLayer = NULL;
Vector<Entity*> *GameScene::enemyList = new Vector<Entity*>;
Vector<Entity*> *GameScene::playerList = new Vector<Entity*>;

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    //layer->setContentSize(Size(BG_WIDTH, BG_HEIGHT));
    //sLayer = ZoomScrollView::create(Size(WIN_WIDTH, WIN_HEIGHT), layer);
    //layer->setPosition(Point(0, -(BG_HEIGHT-WIN_HEIGHT)/2));
    
    menulayer = MenuLayer::create();
    //sLayer->setBounceable(false);
    scene->addChild(layer, 1, GAMELAYER_TAG);
    //("%f, %f", layer->getContentSize().width, layer->getContentSize().height);
    scene->addChild(menulayer, 2, MENULAYER_TAG);
    return scene;
    
}
bool GameScene::init()
{
    bool bRet = false;
    do{
        test = false;
        maxScale = 1.0f;
        minScale = 0.6f;
        srand((unsigned int) time(NULL));
        initBG();
        initFrameCache();
        for (int i=0; i<=5; i++)
        {
            auto testEnemy = EnemyKnight::create(2, RIGHT_EDGE_X- i*90);
            this->addChild(testEnemy, 2);
            auto testEnemy2 = EnemyBlueDragon::create(2, RIGHT_EDGE_X - i*80);
            this->addChild(testEnemy2, 2);
            
            auto testEnemy3 = EnemyBlueDragon::create(1, RIGHT_EDGE_X - i*60);
            this->addChild(testEnemy3, 2);
            auto testEnemy4 = EnemyKnight::create(1, RIGHT_EDGE_X - i*40);
            this->addChild(testEnemy4, 2);
        }
        /*auto testTower = TowerMagic::create(2, 850);
        this->addChild(testTower, 1);
        auto testTower2 = TowerRocket::create(2, 700);
        this->addChild(testTower2, 1);
        auto testTower3 = TowerBarrack::create(2, 500);
        this->addChild(testTower3, 1);
        */
        auto testEnemy2 = EnemyKnight::create(2, RIGHT_EDGE_X-700);
        
        this->addChild(testEnemy2, 2);
        /*
        auto testTower5 = TowerRocket::create(1, 400);
        this->addChild(testTower5, 1);
        auto testTower6 = TowerBarrack::create(1, 600);
        this->addChild(testTower6, 1);
         */
        hero = HeroCat::create();
        this->addChild(hero, 3);
        bRet = true;
        this->scheduleUpdate();
    }while (0);
    return bRet;
}
void GameScene::initFrameCache()
{
    auto frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile("Tower.plist", "Tower.png");
    frameCache->addSpriteFramesWithFile("Enemy.plist", "Enemy.png");
    frameCache->addSpriteFramesWithFile("Effect.plist", "Effect.png");
    frameCache->addSpriteFramesWithFile("Soldier.plist", "Soldier.png");
    frameCache->addSpriteFramesWithFile("Hero.plist", "Hero.png");
}
void GameScene::initBG()
{
    
    m_bgSprite = Sprite::create("background.png");
    scaleNow = 2.0f;
    m_bgSprite->setScale(scaleNow);
    m_bgSprite->setAnchorPoint(Point(0,0));
    m_bgSprite->setPosition(Point::ZERO);
    this->addChild(m_bgSprite, 0);
    auto bgListener = EventListenerTouchAllAtOnce::create();
    bgListener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBGBegan, this);
    bgListener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesBGMoved, this);
    bgListener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesBGEnded, this);
    
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(bgListener, m_bgSprite);
}
void GameScene::onTouchesBGBegan(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event)
{
    if (touches.size()==1)
    {
        nowTouchPoint = touches[0]->getLocation();
        moved = false;
    }
    else if (touches.size()==2)
    {
        Point p1 = touches[0]->getLocation();
        Point p2 = touches[1]->getLocation();
        touchDist = p1.getDistance(p2);
        midPoint = this->convertToNodeSpace((p1+p2)/2);
    }
}
void GameScene::onTouchesBGMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event*)
{
    if (touches.size()==1)
    {
        Point tmpPoint = touches[0]->getLocation();
        Point nowPoint = this->getPosition();
        nowPoint = nowPoint + (tmpPoint - nowTouchPoint);
        this->setPosition(nowPoint);
        nowTouchPoint = tmpPoint;
        moved = true;
    }
    else if (touches.size()==2)
    {
        
        Point p1 = touches[0]->getLocation();
        Point p2 = touches[1]->getLocation();
        float tmpDist = p1.getDistance(p2);
        float tmpScale = (1+ (tmpDist/touchDist-1)*0.4) * this->getScale();
        float oldScale = this->getScale();
        touchDist = tmpDist;
        tmpScale = fmax(minScale, fmin(maxScale, tmpScale));
        Point tmpMidPoint = this->convertToNodeSpace((p1+p2)/2);
        this->setScale(tmpScale);
        this->setPosition(tuningPoint(this->getPosition() + (tmpMidPoint*oldScale-midPoint*tmpScale)));
        midPoint = tmpMidPoint;
    }
}
void GameScene::onTouchesBGEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event*)
{
    Point nowPoint = this->getPosition();
    Point oriPoint = tuningPoint(nowPoint);
    if (!nowPoint.equals(oriPoint)){
        auto moveTo = MoveTo::create(0.5f, oriPoint);
        this->runAction(moveTo);
    }
    if (touches.size()==1 && !moved)
    {
        hero->runToDest(this->convertToNodeSpace(touches[0]->getLocation()));
    }
}

Point GameScene::tuningPoint(Point oriPoint)
{
    Point nowPoint = oriPoint;
    updateEdges();
    nowPoint.x = fmin(maxWidth, nowPoint.x);
    nowPoint.x = fmax(minWidth, nowPoint.x);
    nowPoint.y = fmin(maxHeight, nowPoint.y);
    nowPoint.y = fmax(minHeight, nowPoint.y);
    return nowPoint;
}

void GameScene::updateEdges()
{
    maxWidth = 0;
    minWidth = WIN_WIDTH - this->getScale()*BG_WIDTH;
    maxHeight = 0;
    minHeight = WIN_HEIGHT - this->getScale()*BG_HEIGHT;
}
void GameScene::update(float dt)
{
        
}
bool GameScene::canAddTower(cocos2d::Point touchPoint)
{
    Point localePoint = this->convertToNodeSpace(touchPoint);
    Rect rect = Rect(localePoint.x - 60, localePoint.y - 90, 120, 180);
    if (localePoint.y<LINE1
        || (localePoint.y>LINE1+140 && localePoint.y<LINE2)
        || (localePoint.y>LINE2+140 && localePoint.y<LINE3)
        || localePoint.y>LINE3)
        return false;
    for (Entity* obj : *playerList)
        if (obj->getType()==TYPE_TOWER)
        {
            CCLOG("%f, %f", obj->getBoundingBox().getMidX(), obj->getBoundingBox().getMidY());
            if (obj->getBoundingBox().intersectsRect(rect))
                return false;
        }
    return true;
}
bool GameScene::addTower(std::string towerName, Point touchPoint)
{
    Entity* tower;
    Point location = this->convertToNodeSpace(touchPoint);
    float tmpLine;
    if (location.y>=LINE1 && location.y<=LINE1+140)
        tmpLine = 1;
    else if (location.y>=LINE2 && location.y<=LINE2+140)
        tmpLine = 2;
    else if (location.y>=LINE3 && location.y<+LINE3+140)
        tmpLine = 3;
    else
        return false;
    if (towerName=="Tower_Magic")
    {
        if (menulayer->getGold()<TOWERMAGIC_GOLD)
            return false;
        tower = TowerMagic::create(tmpLine, location.x-52);
        menulayer->minusGold(TOWERMAGIC_GOLD);
    }
    else if (towerName=="Tower_Rocket")
    {
        if (menulayer->getGold()<TOWERROCKET_GOLD)
            return false;
        menulayer->minusGold(TOWERROCKET_GOLD);
        tower = TowerRocket::create(tmpLine, location.x -53);
    }
    else if (towerName=="Tower_Barrack")
    {
        if (menulayer->getGold()<TOWERBARRACK_GOLD)
            return false;
        menulayer->minusGold(TOWERBARRACK_GOLD);
        tower = TowerBarrack::create(tmpLine, location.x -60);
    }
    else return false;

    this->addChild(tower, 1);
    return true;
}
void ZoomScrollView::setMinScale(int _tmpscale)
{
    _minScale = _tmpscale;
}