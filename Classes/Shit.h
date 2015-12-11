//
//  Shit.hpp
//  RushShit
//
//  Created by gjmac-2 on 15/12/11.
//
//

#ifndef Shit_h
#define Shit_h

#include "cocos2d.h"

USING_NS_CC;

class Shit : public Sprite
{
public:
    static Shit* createShit(int x,int y);
    virtual bool initShit(int x,int y);
    void clickToJiao();
    void playShitAnimation();
    
    CC_SYNTHESIZE(int, _x, X);
    CC_SYNTHESIZE(int, _y, Y);
    CC_SYNTHESIZE(int, _type, Type);
};

#endif /* Shit_h */
