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

class ShuiGuan : public Sprite
{
public:
    
    static ShuiGuan* createShuiGuan(int x,int y,int shuiGuanIndex = 0);
    bool initShuiGuan(int x,int y,int shuiGuanIndex);
    
    void changeDirection();
    void changeType(ShuiGuanType type);
    
    CC_SYNTHESIZE(int, _x, X);
    CC_SYNTHESIZE(int, _y, Y);
    CC_SYNTHESIZE(int, _shuiGuanIndex, ShuiGuanIndex);
    CC_SYNTHESIZE(ShuiGuanType, _type, Type);
    CC_SYNTHESIZE(int, _direction, Direction);
};

#endif /* ShuiGuan_h */
