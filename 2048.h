#ifndef _TWO_H_
#define _TWO_H_
#include "Game.h"
#include <iostream>
#include <fstream>
using namespace std;
class Two : public Game
{
protected:
    char gameName[10] = "EzMaze"; //游戏名
    static const int L = 30;      //迷宫大小
    int map[L][L] = {0};          //迷宫初始化
    time_t start, end, last = 0;  //时间计算相关
    int n_x, n_y = 0;             //玩家现在所处坐标

public:
    Two(Person &obj) : Game(obj) { return; }

    void CreateMaze();     //迷宫生成函数
    void updateScene();    //场景更新函数
    void saveMap();        //存档
    void readMap();        //读档
    int menu(int mode);    //菜单函数
    void pause();          //暂停
    void quit();           //结束结算函数
    void ioCatcher();      //用户IO输入捕获
    void move(char op);    //人物移动
    void processControl(); //流程控制函数
};
#endif