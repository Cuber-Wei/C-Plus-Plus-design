#include <iostream>
#include <fstream>
#include <string>
#include "2048.h"
#include "AccountManage.h"
#include "LogIn.h"
#include "Maze.h"
#include "MenuUI.h"
#include "Sign.h"

using namespace std;
Person p;
int menu(int mode = 0)
{
    int n_y = 1;
    system("cls");
    if (p.getUID() == 10000000)
    {
        Menu infobar("", "管理员 admin", LINE_WIDTH, 100, 50);
        n_y = infobar.infoBar(n_y, 3);
    }
    else if (p.getUID() == 0)
    {
        Menu infobar("", "未登录", LINE_WIDTH, 100, 50);
        n_y = infobar.infoBar(n_y, 3);
    }
    else
    {
        string cap = "玩家 ";
        cap += p.getName();
        Menu infobar(cap, to_string(p.getUID()), LINE_WIDTH, 100, 50);
        n_y = infobar.infoBar(n_y);
    }

    if (mode == 0)
    {
        string title0 = "小游戏合集";
        string items0[5] = {
            "1.注册账号",
            "2.登录",
            "3.账号管理",
            "4.选择游戏",
            "5.退出程序"};
        Menu menu1(title0, items0, LINE_WIDTH, 5, 100, 50);
        return menu1.menuUI(n_y);
    }
    else
    {
        string title1 = "选择要玩的游戏:";
        string items1[2] = {
            "1.EzMaze",
            "2.2048"};
        Menu menu2(title1, items1, LINE_WIDTH, 2, 100, 50);
        return menu2.menuUI(n_y);
    }
}
int main()
{
    system("mode con cols=100 lines=50"); //初始化窗口界面大小
    int ch;
    while (1)
    {
        system("cls");
        ch = menu(0);
        switch (ch)
        {
        case 1:
        {
            Sign sign;
            sign.processControl();
            break;
        }
        case 2:
        {
            LogIn login;
            p = login.processControl();
            system("pause");
            break;
        }
        case 3:
        {
            AccountManage am(p);
            am.processControl();
            break;
        }
        case 4:
        {
            ch = menu(1);
            switch (ch)
            {
            case 1:
            {
                Maze maze(p);
                maze.processControl();
                break;
            }
            default:
                break;
            }
            system("pause");
            break;
        }
        case 5:
            exit(0);
        default:
            break;
        }
    }
    system("pause");
    return 0;
}