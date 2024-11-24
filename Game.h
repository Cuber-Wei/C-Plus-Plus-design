#ifndef _GAME_H_
#define _GAME_H_
#include "Person.h"
class Game
{
protected:
    Person *user;
    int currentScore = 0;
    bool isEnd = false, flag = true;

public:
    Game(Person &obj);                 //构造函数
    virtual int menu(int mode) = 0;    //菜单函数
    virtual void pause() = 0;          //暂停
    virtual void quit() = 0;           //结束结算函数
    virtual void updateScene() = 0;    //场景更新函数
    virtual void ioCatcher() = 0;      //用户IO输入捕获
    virtual void processControl() = 0; //流程控制函数
};
Game::Game(Person &obj)
{
    user = &obj;
}

#endif