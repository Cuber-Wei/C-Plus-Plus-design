#ifndef _MAZE_H_
#define _MAZE_H_
#include <vector>
#include <iomanip>
#include <iostream>
#include <ctime>
#include <fstream>
#include "Game.h"
#include "MenuUI.h"
using namespace std;
class Maze : public Game
{
protected:
    char gameName[10] = "EzMaze"; //游戏名
    static const int L = 30;      //迷宫大小
    int map[L][L] = {0};          //迷宫初始化
    time_t start, end, last = 0;  //时间计算相关
    int n_x, n_y = 0;             //玩家现在所处坐标

public:
    Maze(Person &obj) : Game(obj) { return; }

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

void Maze::CreateMaze()
{
    //基于Prim算法生成的随机迷宫
    //最外围设置为路，可以有效的保护里面一层墙体，并防止出界
    for (int i = 0; i < L; i++)
    {
        map[i][0] = 1;
        map[0][i] = 1;
        map[L - 1][i] = 1;
        map[i][L - 1] = 1;
    }

    //墙队列，包括X , Y
    vector<int> X;
    vector<int> Y;

    //任取初始值
    X.push_back(2);
    Y.push_back(2);
    //初始化随机数种子
    srand(time(0));

    //当墙队列为空时结束循环
    while (X.size())
    {
        //在墙队列中随机取一点
        int r = rand() % X.size();
        int x = X[r];
        int y = Y[r];

        //判读上下左右四个方向是否为路
        int count = 0;
        for (int i = x - 1; i < x + 2; i++)
        {
            for (int j = y - 1; j < y + 2; j++)
            {
                if (abs(x - i) + abs(y - j) == 1 && map[i][j] > 0)
                    ++count;
            }
        }

        if (count <= 1)
        {
            map[x][y] = 1;
            //在墙队列中插入新的墙
            for (int i = x - 1; i < x + 2; i++)
            {
                for (int j = y - 1; j < y + 2; j++)
                {
                    if (abs(x - i) + abs(y - j) == 1 && map[i][j] == 0)
                    {
                        X.push_back(i);
                        Y.push_back(j);
                    }
                }
            }
        }

        //删除当前墙
        X.erase(X.begin() + r);
        Y.erase(Y.begin() + r);
    }

    //设置迷宫进出口
    map[2][1] = 3;
    for (int i = L - 3; i >= 0; i--)
    {
        if (map[i][L - 3] == 1)
        {
            map[i][L - 2] = 4;
            break;
        }
    }
    n_x = 2;
    n_y = 1;
}
void Maze::updateScene()
{
    int y = 0;
    //清屏
    system("cls");
    y = menu(1);
    Menu tmp("", "", LINE_WIDTH, 100, 50);
    tmp.gotoMid(y, L * 2);
    //画迷宫
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < L; j++)
        {
            if (map[i][j] == 3)
                cout << setw(2) << setfill(' ') << '@';
            else if (map[i][j] == 1)
                cout << setw(2) << setfill(' ') << ' ';
            else if (map[i][j] == 4)
                cout << setw(2) << setfill(' ') << "P";
            else
                cout << setw(2) << setfill(' ') << "国";
        }
        tmp.gotoMid(++y, L * 2);
    }
}

void Maze::saveMap()
{
    fstream dataFile;
    dataFile.open(user->getPath() + "/game_save/EzMaze.dat", ios::out | ios::binary); //用户数据文件名
    if (dataFile.fail())
    {
        system("cls");
        Menu info("", "文件打开失败!", LINE_WIDTH, 100, 50);
        info.infoBar(3, 3);
        return;
    }
    dataFile.write((char *)&n_x, sizeof(int));
    dataFile.write((char *)&n_y, sizeof(int));
    dataFile.write((char *)&last, sizeof(time_t));
    dataFile.write((char *)&map, L * L * sizeof(int));
    dataFile.close();
}
void Maze::readMap()
{
    fstream dataFile;
    dataFile.open(user->getPath() + "/game_save/EzMaze.dat", ios::in | ios::binary); //用户数据文件名
    if (dataFile.fail())
    {
        system("cls");
        Menu info("", "文件打开失败!", LINE_WIDTH, 100, 50);
        int n_y = info.infoBar(3, 3);
        info.setContent("可能是未存档过!");
        n_y = info.infoBar(n_y, 3);
        info.setContent("暂无存档!现在为您新开一个存档!");
        n_y = info.infoBar(n_y, 3);
        CreateMaze();
        system("pause");
        return;
    }
    dataFile.read((char *)&n_x, sizeof(int));
    dataFile.read((char *)&n_y, sizeof(int));
    dataFile.read((char *)&last, sizeof(time_t));
    dataFile.read((char *)&map, L * L * sizeof(int));
    dataFile.close();
}

int Maze::menu(int m = 1)
{
    system("cls");
    switch (m)
    {
    case 0:
    {
        string title0 = "欢迎来到EzMaze小游戏!";
        string items0[3] = {
            "1.新游戏",
            "2.读取存档",
            "3.返回",
        };
        Menu menu0(title0, items0, LINE_WIDTH, 3, 100, 50);
        return menu0.menuUI(1);
    }
    case 1: //欢迎菜单,介绍游戏内容及玩法
    {
        Menu infoPlay("", "wasd控制人物(@)的移动,q键暂停;'国'为围墙,你需要控制人物走出迷宫取得flag.", LINE_WIDTH, 100, 50);
        int y = infoPlay.infoBar(1, 2);
        infoPlay.setCaption(user->getName());
        infoPlay.setContent(to_string(user->getUID()));
        return infoPlay.infoBar(y, 2);
    }
    break;
    case 2: //暂停菜单
    {
        string title1 = "已暂停";
        string items1[2] = {
            "1.继续游戏",
            "2.退出游戏",
        };
        Menu menu2(title1, items1, LINE_WIDTH, 2, 100, 50);
        return menu2.menuUI(1);
    }
    case 3: //退出游戏时的菜单
    {
        string title1 = "存档?";
        string items1[1] = {
            "是否存档?[Y/n]",
        };
        Menu menu2(title1, items1, LINE_WIDTH, 1, 100, 50);
        return menu2.menuUI(1, 0);
    }
    }
    return -1;
}
void Maze::pause() //暂停,记录暂停的时间
{
    time_t tmp = time(0);
    last += tmp - start;
    int ch = menu(2);
    if (ch == 1)
        start = time(0);
    else
    {
        flag = !flag;
        quit(); //非正常退出
    }
}
void Maze::move(char op)
{
    switch (op)
    {
    case 'w':
        if (map[n_x - 1][n_y] != 0) //向上有通路
        {
            map[n_x][n_y] = 1;
            n_x--;
        }
        break;
    case 'a':
        if (map[n_x][n_y - 1] != 0) //向左有通路
        {
            map[n_x][n_y] = 1;
            n_y--;
        }
        break;
    case 's':
        if (map[n_x + 1][n_y] != 0) //向上有通路
        {
            map[n_x][n_y] = 1;
            n_x++;
        }
        break;
    case 'd':
        if (map[n_x][n_y + 1] != 0) //向上有通路
        {
            map[n_x][n_y] = 1;
            n_y++;
        }
        break;
    default:
        break;
    }
    if (map[n_x][n_y] == 4) //到达出口,游戏结束
        isEnd = true;
    map[n_x][n_y] = 3;
}
void Maze::ioCatcher()
{
input:
    char ch = ' ';
    ch = getch();
    switch (ch)
    {
    case 'w':
    case 'a':
    case 's':
    case 'd':
        move(ch);
        break;
    case 'q': // q键,暂停
        pause();
        break;
    default:
        goto input;
    }
}
void Maze::quit()
{
    Menu info("", "", LINE_WIDTH, 100, 50);
    system("cls");
    if (isEnd) //游戏结束
    {
        end = time(0);
        last += end - start;
        int finalScore = 100000 - last * 100;
        info.setCaption("您最终得分为:");
        info.setContent(to_string(finalScore));
        info.infoBar(3, 1);
        system("pause");
        user->updateGameData(gameName, finalScore);
        remove((user->getPath() + "/game_save/EzMaze.dat").c_str()); //删除存档
    }
    else //游戏中断,询问是否存档
    {
        system("cls");
        int ch = menu(3);
        if ((ch + '0') == 'Y' || (ch + '0') == 'y') //存档操作
            saveMap();
    }
    system("cls");
    info.setContent("谢谢游玩!");
    info.infoBar(3, 3);
}
void Maze::processControl()
{
    system("cls");
    if (user->getUID() == 0)
    {
        cout << "请先登录!" << endl;
        system("pause");
        return;
    }
    int way = menu(0);
    switch (way)
    {
    case 1: //新游戏
        CreateMaze();
        break;
    case 2: //读取存档
        readMap();
        break;
    case 3: //返回主菜单
        return;
    }
    system("cls");
    menu(1);
    start = time(0);
    while (!isEnd && flag)
    {
        updateScene(); //输出游戏信息
        ioCatcher();   //捕获IO输入
    }
    if (!flag)
        return;
    quit(); //正常结算退出
    system("pause");
}

#endif