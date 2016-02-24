//
//  GameScene.cpp
//  RushShit
//
//  Created by gjmac-2 on 15/12/7.
//
//

#include "GameScene.h"
#include <iostream>
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

GameScene::GameScene():xCount(COLUMN_COUNT),yCount(ROW_COUNT),level(1)
{
    winSize = Director::getInstance()->getVisibleSize();
    shuiGuanWidth = winSize.width/8;
    startX = shuiGuanWidth/2;
    startY = 0;
    
    score = 0;
    targetScore = level * 100 + 100;
    lastTimes = level * 30 + 30;
}

GameScene::~GameScene()
{
    
}

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shuiguan.plist", "shuiguan.pvr.ccz");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shit.plist","shit.pvr.ccz");
    
    auto node = CSLoader::getInstance()->createNode("MainLayer.csb");
    addChild(node);
    
    _mainGame = node->getChildByName("Panel_1")->getChildByName("Panel_game");
    
    initShuiGuan();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch* touch,Event* event){
        auto touchPos = touch->getLocation();
        touchPos = _mainGame->convertToNodeSpace(touchPos);
        printf("水管的大小width:%f,height:%f",shuiGuanVector[0][0]->getBoundingBox().size.width,shuiGuanVector[0][0]->getBoundingBox().size.height);
        for (int y = 0 ; y < yCount; ++y) {
            for (int x = 0; x < xCount; ++x) {
                if (shuiGuanVector[x][y]->getBoundingBox().containsPoint(touchPos)) {
                    if (shitArray[x][y] != nullptr) {
                        shitArray[x][y]->clickToJiao();
                    }else{
                        shuiGuanVector[x][y]->changeDirection();
                        check();
                    }
                    return false;
                }
            }
        }
        
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void GameScene::initShuiGuan()
{
    for (int y = 0; y<yCount; ++y) {
        for (int x = 0; x<xCount; ++x) {
            shuiGuanVector[x][y] = createAndDropShuiGuan(x, y);
            shitArray[x][y] = nullptr;
        }
    }
    check();
    addShit();
}

ShuiGuan* GameScene::createAndDropShuiGuan(int x, int y)
{
    auto endPosition = getEndPosition(x, y);
    auto startPosition = Vec2(endPosition.x,winSize.height/2 + shuiGuanWidth/2 * y);
    auto shuiguan = ShuiGuan::createShuiGuan(x, y);
    shuiguan->setPosition(startPosition);
    auto moveAction = MoveTo::create(startPosition.y / winSize.height/2, endPosition);
    shuiguan->runAction(moveAction);
    _mainGame->addChild(shuiguan);
    return shuiguan;
}

Vec2 GameScene::getEndPosition(int x, int y)
{
    float endX = startX + shuiGuanWidth/2 + shuiGuanWidth * x;
    float endY = startY + shuiGuanWidth/2 + shuiGuanWidth * y;
    return Vec2(endX,endY);
}

void GameScene::check()
{
    unschedule("removeShuiGuan");
    clearShuiGuan();
    for (int y = 0; y<yCount; ++y) {
        checkShuiGuan(shuiGuanVector[0][y],ConnectDir::RIGHT,ShuiGuanType::RED);
    }
    
    for (int y = 0; y<yCount; ++y) {
        checkShuiGuan(shuiGuanVector[xCount-1][y], ConnectDir::LEFT, ShuiGuanType::GREEN);
    }
    for (int y = 0; y < yCount; ++y) {
        for (int x = 0; x < xCount; ++x) {
            if (shuiGuanVector[x][y]->getType() == ShuiGuanType::LIGHT_GREEN) {
                scheduleOnce([=](float dt){
                    removeConnectShuiGuan();
                }, 1, "removeShuiGuan");
                return;
            }
        }
    }
}

void GameScene::checkShuiGuan(ShuiGuan* shuiguan,ConnectDir dir,ShuiGuanType type)
{
    if (shuiguan->getType() == type or shuiguan->getType() == ShuiGuanType::LIGHT_GREEN) {
        return;
    }
    int x = shuiguan->getX();
    int y = shuiguan->getY();
    auto canLeft = isConnectLeft(shuiguan);
    auto canUp = isConnectUp(shuiguan);
    auto canRight = isConnectRight(shuiguan);
    auto canDown = isConnectDown(shuiguan);
    
    if ((canLeft and dir == ConnectDir::RIGHT) or
        (canUp and dir == ConnectDir::DOWN) or
        (canRight and dir == ConnectDir::LEFT) or
        (canDown and dir == ConnectDir::UP)) {
        
        if (shuiguan->getType() == ShuiGuanType::RED && type == ShuiGuanType::GREEN) {
            type = ShuiGuanType::LIGHT_GREEN;
        }
        
        shuiguan->changeType(type);
    }
    
    if (shuiguan->getType() != type) {
        return;
    }

    if (canLeft and shuiguan->getX() > 0) {
        checkShuiGuan(shuiGuanVector[x-1][y], ConnectDir::LEFT, type);
    }
    
    if (canUp and shuiguan->getY() < yCount - 1) {
        checkShuiGuan(shuiGuanVector[x][y+1], ConnectDir::UP, type);
    }
    
    if (canRight and shuiguan->getX() < xCount - 1) {
        checkShuiGuan(shuiGuanVector[x+1][y], ConnectDir::RIGHT, type);
    }
    
    if (canDown and shuiguan->getY() > 0) {
        checkShuiGuan(shuiGuanVector[x][y-1], ConnectDir::DOWN, type);
    }
}

void GameScene::clearShuiGuan()
{
    for (int y = 0; y < yCount; ++y) {
        for (int x = 0; x < xCount; ++x) {
            shuiGuanVector[x][y]->changeType(ShuiGuanType::GRAY);
        }
    }
}

void GameScene::removeConnectShuiGuan()
{
    for (int y = 0; y < yCount; ++y) {
        for (int x = 0; x < xCount; ++x) {
            if (shuiGuanVector[x][y]->getType() == ShuiGuanType::LIGHT_GREEN) {
                
                if (shitArray[x][y] != nullptr) {
                    shitArray[x][y] = nullptr;
                }
                shuiGuanVector[x][y]->removeFromParent();
                shuiGuanVector[x][y] = nullptr;
            }
        }
    }
    
    dropShuiGuan();
}

void GameScene::dropShuiGuan()
{
    int emptyList[xCount];
    for (int x = 0; x < xCount; ++x) {
        int emptyCount = 0;
        int newY = 0;
        for (int y = 0; y < yCount; ++y) {
            if (shuiGuanVector[x][y] == nullptr) {
                emptyCount++;
            }else{
                if (emptyCount > 0) {
                    newY = y - emptyCount;
                    shuiGuanVector[x][newY] = shuiGuanVector[x][y];
                    shuiGuanVector[x][newY]->setY(newY);
                    shuiGuanVector[x][y] = nullptr;
                    
                    if (shitArray[x][y] != nullptr) {
                        shitArray[x][newY] = shitArray[x][y];
                        shitArray[x][newY]->setY(newY);
                        shitArray[x][y] = nullptr;
                    }
                    
                    auto endPosition = getEndPosition(x, newY);
                    auto moveAction = MoveTo::create(shuiGuanVector[x][newY]->getPositionY() / winSize.height/2, endPosition);
                    shuiGuanVector[x][newY]->stopAllActions();
                    shuiGuanVector[x][newY]->runAction(moveAction);
                }
            }
        }
        emptyList[x] = emptyCount;
    }
    for (int i = 0; i < xCount; ++i) {
        for (int j = 0; j < emptyList[i]; ++j) {
            shuiGuanVector[i][yCount - 1 - j] = createAndDropShuiGuan(i, yCount - 1 - j);
        }
    }
    check();
    addShit();
}

//添加便便
void GameScene::addShit()
{
    bool isAdd = false;
    while (!isAdd) {
        auto random = RandomHelper::random_int(0, xCount*yCount - 1);
        auto y = (int)(random / xCount);
        auto x = random - y * xCount;
        
        if (shitArray[x][y] == nullptr) {
            auto shuiguan = shuiGuanVector[x][y];
            auto endPosition = Vec2(shuiguan->getContentSize().width/2,shuiguan->getContentSize().height/2);
            auto shit = Shit::createShit(x, y);
            shit->setPosition(Vec2(endPosition.x,winSize.height - shuiguan->getPositionY() + shit->getContentSize().height/2));
            auto moveAction = MoveTo::create(0.3, endPosition);
            shit->runAction(moveAction);
            shuiguan->addChild(shit);
            shitArray[x][y] = shit;
            isAdd = true;
        }
    }
}

bool GameScene::isConnectLeft(ShuiGuan* shuiguan)
{
    for (int i = 0; i < 4; ++i) {
        if ((int)shuiguan->getDirection() == leftDirection[shuiguan->getShuiGuanIndex()][i]) {
            return true;
        }
    }
    return false;
}

bool GameScene::isConnectUp(ShuiGuan* shuiguan)
{
    for (int i = 0; i < 4; ++i) {
        if ( (int)shuiguan->getDirection() == upDirection[shuiguan->getShuiGuanIndex()][i]) {
            return true;
        }
    }

    return false;
}

bool GameScene::isConnectRight(ShuiGuan* shuiguan)
{
    for (int i = 0; i < 4; ++i) {
        if ( (int)shuiguan->getDirection() == rightDirection[shuiguan->getShuiGuanIndex()][i]) {
            return true;
        }
    }

    return false;
}

bool GameScene::isConnectDown(ShuiGuan* shuiguan)
{
    for (int i = 0; i < 4; ++i) {
        if ( (int)shuiguan->getDirection() == downDirection[shuiguan->getShuiGuanIndex()][i]) {
            return true;
        }
    }

    return false;
}
