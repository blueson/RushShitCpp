//
//  Shit.cpp
//  RushShit
//
//  Created by gjmac-2 on 15/12/11.
//
//

#include "Shit.h"

Shit* Shit::createShit(int x, int y)
{
    auto shit = new Shit();
    if (shit && shit->initShit(x, y)) {
        shit->autorelease();
        return shit;
    }
    CC_SAFE_DELETE(shit);
    shit = nullptr;
    
    return nullptr;
}

bool Shit::initShit(int x, int y)
{
    if (!Sprite::initWithSpriteFrameName("shit0_0.png")) {
        return false;
    }
    
    setX(x);
    setY(y);
    
    auto type = RandomHelper::random_int(0, 3);
    setType(type);
    setScale(0.5);
    playShitAnimation();
    
    return true;
}

void Shit::playShitAnimation()
{
    auto animationName =StringUtils::format("shit%d",getType());
    auto animation = AnimationCache::getInstance()->getAnimation(animationName);
    if (!animation) {
        Vector<SpriteFrame*> frameVector;
        for (int i = 0; i<2; ++i) {
            auto str = StringUtils::format("shit%d_%d.png",getType(),i);
            auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
            frameVector.pushBack(frame);
        }
        
        AnimationCache::getInstance()->addAnimation(Animation::createWithSpriteFrames(frameVector), animationName);
        animation = AnimationCache::getInstance()->getAnimation(animationName);
    }
    animation->setDelayPerUnit(0.1);
    
    auto animate = Animate::create(animation);
    auto action = RepeatForever::create(animate);
    this->runAction(action);
}

void Shit::clickToJiao()
{
    stopAllActions();
    setSpriteFrame("shit_jiao.png");
    auto scaleAction1 = ScaleTo::create(0.1, 1);
    auto scaleAction2 = ScaleTo::create(0.1, 0.5);
    
    runAction(Sequence::create(scaleAction1,scaleAction2,CallFunc::create(CC_CALLBACK_0(Shit::playShitAnimation, this)), NULL));
}
