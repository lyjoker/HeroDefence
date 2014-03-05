//
//  Common.h
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#ifndef __HeroDeffence__Common__
#define __HeroDeffence__Common__

const float WIN_WIDTH = 1136;
const float WIN_HEIGHT = 640;
const float BG_WIDTH = 2200;
const float BG_HEIGHT = 1104;
const float LEFT_EDGE_X = 0;
const float RIGHT_EDGE_X = BG_WIDTH;
const float LINE1 = 175;
const float LINE2 = 670;
const float LINE3 = 900;


#define ENEMY_FACTION 0
#define PLAYER_FACTION 1

#define STATUS_RUNNING 1
#define STATUS_ATTACKING 2
#define STATUS_DONOTHING 3
#define STATUS_COOLDOWN 4
#define STATUS_DEAD 5

#define MAGICTOWER_HP 1500
#define MAGICTOWER_ATT 150
#define MAGICTOWER_ATTSPEED 2.5f
#define MAGICTOWER_RANGE 500

#define TYPE_FLY 111
#define TYPE_WALK 222
#define TYPE_TOWER 333
#define TYPE_HERO 444

#define TYPE_MAGICTOWER 1
#define TYPE_ROCKETTOWER 2
#define TYPE_BARRACKTOWER 3

#define TOWERICON_X1 400
#define TOWERICON_X2 550
#define TOWERICON_X3 700
#define TOWERICON_X4 850
#define TOWERICON_Y  100
#define HEROICON_X 200
#define HEROICON_Y 100



#define GAMELAYER_TAG 101
#define MENULAYER_TAG 102

#define TOWERMAGIC_GOLD 300
#define TOWERROCKET_GOLD 450
#define TOWERBARRACK_GOLD 500

const int NeedGold[] = {TOWERMAGIC_GOLD, TOWERROCKET_GOLD, TOWERBARRACK_GOLD};
#endif /* defined(__HeroDeffence__Common__) */
