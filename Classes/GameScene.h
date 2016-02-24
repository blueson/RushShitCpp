//
//  GameScene.h
//  RushShit
//
//  Created by gjmac-2 on 15/12/7.
//
//

#ifndef GameScene_h
#define GameScene_h

#include "cocos2d.h"
#include "ShuiGuan.h"
#include "Shit.h"

USING_NS_CC;

#define ROW_COUNT 9
#define COLUMN_COUNT 7

enum ConnectDir
{
    LEFT = 0,
    UP,
    RIGHT,
    DOWN
};

class GameScene : public Layer
{
public:
    GameScene();
    ~GameScene();
    static Scene* createScene();
    virtual bool init();
    
    Vec2 getEndPosition(int x,int y);
    ShuiGuan* createAndDropShuiGuan(int x,int y);
    void initShuiGuan();
    //检查两边水管是否连接
    void check();
    //检查水管的递归方法
    void checkShuiGuan(ShuiGuan* shuiguan,ConnectDir dir,ShuiGuanType type);
    //清空水管
    void clearShuiGuan();
    //删除连接水管
    void removeConnectShuiGuan();
    //掉落新水管
    void dropShuiGuan();
    
    void addShit();
    
    bool isConnectLeft(ShuiGuan* shuiguan);
    bool isConnectUp(ShuiGuan* shuiguan);
    bool isConnectRight(ShuiGuan* shuiguan);
    bool isConnectDown(ShuiGuan* shuiguan);
    
    CREATE_FUNC(GameScene);
    
private:
    int xCount; //水管的列数
    int yCount; //水管的行数
    int level;  //当前的等级
    int score;  //当前的分数
    int targetScore;    //目标分数
    int lastTimes;  //剩余的时间
    float startX;
    float startY;
    float shuiGuanWidth;
    Size winSize;
    Node* _mainGame;
    
    ShuiGuan* shuiGuanVector[COLUMN_COUNT][ROW_COUNT];
    Shit* shitArray[COLUMN_COUNT][ROW_COUNT];
    
    int leftDirection[4][4] = {{-1,1,-1,3},{0,1,2,3},{-1,-1,2,3},{0,-1,2,3}};
    int upDirection[4][4] = {{0,-1,2,-1},{0,1,2,3},{0,-1,-1,3},{0,1,-1,3}};
    int rightDirection[4][4] = {{-1,1,-1,3},{0,1,2,3},{0,1,-1,-1},{0,1,2,-1}};
    int downDirection[4][4] = {{0,-1,2,-1},{0,1,2,3},{-1,1,2,-1},{-1,1,2,3}};
};

#endif /* GameScene_h */
