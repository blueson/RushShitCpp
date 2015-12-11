//
//  ShuiGuan.h
//  RushShit
//
//  Created by gjmac-2 on 15/12/7.
//
//

#ifndef ShuiGuan_h
#define ShuiGuan_h

#include "cocos2d.h"

USING_NS_CC;

enum ShuiGuanType
{
    GRAY = 0,
    RED,
    GREEN,
    LIGHT_GREEN
};

enum ShuiGuanTypeIndex
{
    YIZHI = 0,
    SHIZHI,
    ZHIJIAO,
    TZHI
};

class ShuiGuan : public Sprite
{
public:
    
    static ShuiGuan* createShuiGuan(int x,int y);
    bool initShuiGuan(int x,int y);
    
    void changeDirection();
    void changeType(ShuiGuanType type);
    
    CC_SYNTHESIZE(int, _x, X);
    CC_SYNTHESIZE(int, _y, Y);
    CC_SYNTHESIZE(ShuiGuanTypeIndex, _shuiGuanIndex, ShuiGuanIndex);
    CC_SYNTHESIZE(ShuiGuanType, _type, Type);
    CC_SYNTHESIZE(int, _direction, Direction);
};

#endif /* ShuiGuan_h */
