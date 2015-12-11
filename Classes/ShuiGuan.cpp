//
//  ShuiGuan.cpp
//  RushShit
//
//  Created by gjmac-2 on 15/12/7.
//
//

#include "ShuiGuan.h"


ShuiGuan* ShuiGuan::createShuiGuan(int x, int y)
{
    auto shuiGuan = new ShuiGuan();
    if (shuiGuan and shuiGuan->initShuiGuan(x,y)) {
        shuiGuan->autorelease();
        return shuiGuan;
    }
    
    CC_SAFE_DELETE(shuiGuan);
    shuiGuan = nullptr;
    
    return nullptr;
}

bool ShuiGuan::initShuiGuan(int x, int y)
{
    auto random = RandomHelper::random_int(0, 100);
    ShuiGuanTypeIndex shuiguanTypeIndex;
    if (random < 5) {
        shuiguanTypeIndex = ShuiGuanTypeIndex::SHIZHI;
    }else if (random < 25){
        shuiguanTypeIndex = ShuiGuanTypeIndex::TZHI;
    }else if (random < 65){
        shuiguanTypeIndex = ShuiGuanTypeIndex::YIZHI;
    }else{
        shuiguanTypeIndex = ShuiGuanTypeIndex::ZHIJIAO;
    }
    
    auto str = StringUtils::format("shuiguan0_%d.png",shuiguanTypeIndex);
    if (!Sprite::initWithSpriteFrameName(str.c_str())) {
        return false;
    }
    
    setX(x);
    setY(y);
    setShuiGuanIndex(shuiguanTypeIndex);
    setType(ShuiGuanType::GRAY);
    
    return true;
}

void ShuiGuan::changeDirection()
{
    if (_direction < 3) {
        _direction++;
    }else{
        _direction = 0;
    }
    stopAllActions();
    runAction(RotateTo::create(0.1,_direction*90));
}

void ShuiGuan::changeType(ShuiGuanType type)
{
    setType(type);
    auto str = StringUtils::format("shuiguan%d_%d.png",(int)getType(),getShuiGuanIndex());
    
    setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(str));
}