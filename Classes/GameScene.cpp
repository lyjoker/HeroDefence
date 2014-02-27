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

//Size visibleSize = Director::getInstance()->getVisibleSize();
float MAP_POS_X = WIN_WIDTH / 2;
float MAP_POS_Y = WIN_HEIGHT / 2;

MenuLayer* GameScene::menulayer = NULL;
Vector<Entity*> *GameScene::enemyList = new Vector<Entity*>;
Vector<Entity*> *GameScene::playerList = new Vector<Entity*>;

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    menulayer = MenuLayer::create();
    scene->addChild(layer);
    scene->addChild(menulayer);
    return scene;
    
}
bool GameScene::init()
{
    bool bRet = false;
    do{
        srand((unsigned int) time(NULL));
        initBG();
        initFrameCache();
        nowTime = 0;
        for (int i=0; i<=5; i++)
        {
            auto testEnemy = EnemyKnight::create(2, RIGHT_EDGE_X- i*100);
            this->addChild(testEnemy, 2);
            auto testEnemy2 = EnemyBlueDragon::create(2, RIGHT_EDGE_X - i*90);
            this->addChild(testEnemy2, 2);
        }
        auto testTower = TowerMagic::create(2, 1100);
        this->addChild(testTower, 1);
        auto testTower2 = TowerMagic::create(2, 900);
        this->addChild(testTower2, 1);
        auto testTower3 = TowerBarrack::create(2, 400);
        this->addChild(testTower3, 1);

        auto testEnemy2 = EnemyKnight::create(2, RIGHT_EDGE_X-700);
        
        this->addChild(testEnemy2, 2);
        
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
    m_bgSprite->setPosition(Point(BG_WIDTH, BG_HEIGHT));
    this->addChild(m_bgSprite, 0);
    this->setPosition(-BG_WIDTH + WIN_WIDTH/2,-BG_HEIGHT + WIN_HEIGHT/2);
    auto bgListener = EventListenerTouchOneByOne::create();
    bgListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBGBegan, this);
    bgListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchBGMoved, this);
    bgListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchBGEnded, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(bgListener, m_bgSprite);
    //getEventDispatcher()->addEventListenerWithFixedPriority(bgListener, 100);
}
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
}
void GameScene::update(float dt)
{
    nowTime+=dt;
    if (menulayer!=NULL)
        menulayer->timeDisplayer->setString(StringUtils::format("%.2f", nowTime));
    
}