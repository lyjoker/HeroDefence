//
//  Common.h
//  HeroDeffence
//
//  Created by Yuan Lin on 2/27/14.
//
//

#ifndef __HeroDeffence__Common__
#define __HeroDeffence__Common__

const float WIN_WIDTH = 1152;
const float WIN_HEIGHT = 640;
const float BG_WIDTH = 1100;
const float BG_HEIGHT = 552;
const float LEFT_EDGE_X = 0;
const float RIGHT_EDGE_X = BG_WIDTH * 2;
const float LINE1 = 175;
const float LINE2 = 670;
const float LINE3 = 900;

#define ENEMY_FACTION 0
#define PLAYER_FACTION 1

#define STATUS_RUNNING 1
#define STATUS_ATTACKING 2
#define STATUS_DONOTHING 3
#define STATUS_COOLDOWN 4

#define MAGICTOWER_HP 1500
#define MAGICTOWER_ATT 150
#define MAGICTOWER_ATTSPEED 2.5f
#define MAGICTOWER_RANGE 500

#define TYPE_FLY 111
#define TYPE_WALK 222
#define TYPE_TOWER 333

#endif /* defined(__HeroDeffence__Common__) */
