#ifndef _MODUEL_H_
#define _MODUEL_H_
#include <string>
#include <iostream>
using namespace std;
class Moduel
{
protected:
    string name; //模块名

public:
    virtual int menu(int start_y, int mode) = 0; //菜单
    void processControl() { return; };           //流程控制函数
};

#endif