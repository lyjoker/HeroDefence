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


USING_NS_CC;
USING_NS_CC_EXT;

//Size visibleSize = Director::getInstance()->getVisibleSize();
float MAP_POS_X = WIN_WIDTH / 2;
float MAP_POS_Y = WIN_HEIGHT / 2;

MenuLayer* GameScene::menulayer = NULL;
ZoomScrollView* GameScene::sLayer = NULL;
Vector<Entity*> *GameScene::enemyList = new Vector<Entity*>;
Vector<Entity*> *GameScene::playerList = new Vector<Entity*>;

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    layer->setContentSize(Size(BG_WIDTH, BG_HEIGHT));
    layer->setScale(0.6f);
    layer->setContentSize(layer->getContentSize()*0.6);
    sLayer = ZoomScrollView::create(Size(WIN_WIDTH, WIN_HEIGHT), layer);
    //layer->setPosition(Point(0, -(BG_HEIGHT-WIN_HEIGHT)/2));
    
    menulayer = MenuLayer::create();
    sLayer->setBounceable(false);
    scene->addChild(sLayer);
    CCLOG("%f, %f", layer->getContentSize().width, layer->getContentSize().height);
    scene->addChild(menulayer);
    return scene;
    
}
bool GameScene::init()
{
    bool bRet = false;
    do{
        test = false;
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
        auto testTower = TowerMagic::create(2, 850);
        this->addChild(testTower, 1);
        auto testTower2 = TowerRocket::create(2, 700);
        this->addChild(testTower2, 1);
        auto testTower3 = TowerBarrack::create(2, 500);
        this->addChild(testTower3, 1);
        
        auto testEnemy2 = EnemyKnight::create(2, RIGHT_EDGE_X-700);
        
        this->addChild(testEnemy2, 2);
        
        auto testTower5 = TowerRocket::create(1, 400);
        this->addChild(testTower5, 1);
        auto testTower6 = TowerBarrack::create(1, 600);
        this->addChild(testTower6, 1);
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
}
void GameScene::initBG()
{
    
    m_bgSprite = Sprite::create("background.png");
    scaleNow = 2.0f;
    m_bgSprite->setScale(scaleNow);
    m_bgSprite->setAnchorPoint(Point(0,0));
    m_bgSprite->setPosition(Point::ZERO);
    this->addChild(m_bgSprite, 0);
    //this->setContentSize(Size(2200,1100));
    /*auto bgListener = EventListenerTouchOneByOne::create();
    bgListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBGBegan, this);
    bgListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchBGMoved, this);
    bgListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchBGEnded, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(bgListener, m_bgSprite);
    //getEventDispatcher()->addEventListenerWithFixedPriority(bgListener, 100);*/
}

/*
bool GameScene::onTouchBGBegan(Touch* touch, Event* event)
{
    nowTouchPoint = touch->getLocation();
    return true;
}
void GameScene::onTouchBGMoved(Touch* touch, Event* event)
{
    Point tmpPoint = touch->getLocation();
    //layerMove(nowTouchPoint.x - tmpPoint.x, -nowTouchPoint.y + tmpPoint.y);
    Point nowPoint = this->getPosition();
    nowPoint = nowPoint + (tmpPoint - nowTouchPoint);
    this->setPosition(nowPoint);
    nowTouchPoint = tmpPoint;
}
void GameScene::onTouchBGEnded(Touch* touch, Event* event)
{
    Point nowPoint = this->getPosition();
    Point oriPoint = nowPoint;
    updateEdges();
    nowPoint.x = fmin(maxWidth, nowPoint.x);
    nowPoint.x = fmax(minWidth, nowPoint.x);
    nowPoint.y = fmin(maxHeight, nowPoint.y);
    nowPoint.y = fmax(minHeight, nowPoint.y);
    if (!nowPoint.equals(oriPoint)){
        auto moveTo = MoveTo::create(0.5f, nowPoint);
        this->runAction(moveTo);
    }
}
void GameScene::updateEdges()
{
    maxWidth = 0;
    minWidth = WIN_WIDTH - scaleNow * BG_WIDTH;
    maxHeight = 0;
    minHeight = WIN_HEIGHT - scaleNow * BG_HEIGHT;
}*/
void GameScene::update(float dt)
{
    //nowTime+=dt;
    //if (menulayer!=NULL)
        //menulayer->timeDisplayer->setString(StringUtils::format("%.2f", nowTime));
    if (test) return;
    test = true;
    sLayer->setMinScale(0.5f);

    CCLOG("%f", sLayer->getContentSize().width);
    
}
void ZoomScrollView::setMinScale(int _tmpscale)
{
    _minScale = _tmpscale;
}