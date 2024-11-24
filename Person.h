#ifndef _PERSON_H_
#define _PERSON_H_
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <iomanip>
#include "DataStruct.h"
#include "MenuUI.h"
using namespace std;
#define GAME_NUM 4
class Person
{
private:
    Account acc;
    GameInfo game;
    int uid = 0;            //账号uid
    string path = "./data"; //用户文件路径

public:
    Person();                        //默认构造函数
    Person(int uidIn, int mode);     //构造函数
    Person(const Person &obj);       //拷贝构造函数
    Person &operator=(Person right); //赋值号重载
    //接口函数
    char *getName();
    string getPath();
    int *getBestScores();
    int getUID();
    void setUID(int uidIn);
    //数据更新函数
    void updateAccData(const Account &acc);
    void updateGameData(char gameName[10], int score);
    void saveData(); //数据保存
    //数据展示函数
    int showGamed(int start);
    int showInfo(int start);
};
Person::Person() { return; }
Person::Person(int uid, int mode = 0)
{
    fstream dataFile;
    setUID(uid);                                                                                             // uid保存                                                                            //拼接数据文件名
    dataFile.open(this->path + "/" + to_string(uid) + "/" + to_string(uid) + ".dat", ios::in | ios::binary); //用户数据文件名
    if (dataFile.fail())
    {
        if (mode == 0) //通常构造
        {
            system("cls");
            Menu info("", "文件打开失败!", LINE_WIDTH, 100, 50);
            info.infoBar(3, 3);
            system("pause");
            return;
        }
        else if (mode == 2) //单个尝试模式
        {
            system("cls");
            Menu info("", "此uid不存在!", LINE_WIDTH, 100, 50);
            info.infoBar(3, 3);
            this->setUID(-1);
            system("pause");
            return;
        }
        else //循环尝试模式
            return;
    }
    dataFile.read((char *)&acc, sizeof(Account));
    for (int i = 0; i < GAME_NUM; i++)
    {
        dataFile.read((char *)&game.gameNames[i], 10 * sizeof(char));
        dataFile.read((char *)&game.bestScores[i], sizeof(int));
    }
    dataFile.close();
}
Person::Person(const Person &obj)
{
    strcpy(this->acc.name, obj.acc.name);
    strcpy(this->acc.password, obj.acc.password);
    for (int i = 0; i < GAME_NUM; i++)
    {
        strcpy(this->game.gameNames[i], obj.game.gameNames[i]);
        this->game.bestScores[i] = obj.game.bestScores[i];
    }
    uid = obj.uid;
}
Person &Person::operator=(Person right)
{
    fstream dataFile;
    setUID(right.uid);                                                                                       // uid保存                                                                            //拼接数据文件名
    dataFile.open(this->path + "/" + to_string(uid) + "/" + to_string(uid) + ".dat", ios::in | ios::binary); //用户数据文件名
    if (dataFile.fail())
    {
        system("cls");
        Menu info("", "文件打开失败!", LINE_WIDTH, 100, 50);
        info.infoBar(3, 3);
        system("pause");
    }
    dataFile.read((char *)&acc, sizeof(Account));
    for (int i = 0; i < GAME_NUM; i++)
    {
        dataFile.read((char *)&game.gameNames[i], 10 * sizeof(char));
        dataFile.read((char *)&game.bestScores[i], sizeof(int));
    }
    dataFile.close();
    return *this;
}
char *Person::getName() { return acc.name; }
string Person::getPath() { return path + "/" + to_string(uid); }
int *Person::getBestScores() { return game.bestScores; }
int Person::getUID() { return uid; }
void Person::setUID(int uidIn) { uid = uidIn; }
void Person::saveData()
{
    fstream dataFile;
    dataFile.open(this->getPath() + "/" + to_string(uid) + ".dat", ios::out | ios::binary); //用户数据文件名
    if (dataFile.fail())
    {
        system("cls");
        Menu info("", "文件打开失败!", LINE_WIDTH, 100, 50);
        info.infoBar(3, 3);
        system("pause");
        return;
    }
    dataFile.write((char *)&acc, sizeof(Account));
    for (int i = 0; i < GAME_NUM; i++)
    {
        dataFile.write((char *)&game.gameNames[i], 10 * sizeof(char));
        dataFile.write((char *)&game.bestScores[i], sizeof(int));
    }
    dataFile.close();
}
void Person::updateAccData(const Account &acc)
{
    strcpy(this->acc.name, acc.name);
    strcpy(this->acc.password, acc.password);
    saveData();
}
void Person::updateGameData(char gameName[10], int score)
{
    for (int i = 0; i < GAME_NUM; i++)
    {
        if (strcmp(this->game.gameNames[i], gameName) == 0) //游戏已玩过，更新数据
        {
            if (this->game.bestScores[i] < score) //游戏得分新高
            {
                this->game.bestScores[i] = score;
                break;
            }
        }
        else if (strcmp(this->game.gameNames[i], "") == 0) //新游戏，添加内容
        {
            strcpy(this->game.gameNames[i], gameName);
            this->game.bestScores[i] = score;
            break;
        }
    }
    saveData();
}
int Person::showInfo(int start)
{
    string content = "玩家";
    content += to_string(this->uid);
    content += "信息";
    Menu info("", content, LINE_WIDTH, 100, 50);
    int n_y = info.infoBar(start, 3);
    info.setCaption("昵称:");
    info.setContent(acc.name);
    n_y = info.infoBar(n_y);
    return n_y;
}
int Person::showGamed(int start)
{
    string content = "玩家";
    content += to_string(this->uid);
    content += "玩过的游戏";
    Menu info("", content, LINE_WIDTH, 100, 50);
    int n_y = info.infoBar(start, 3);

    info.setCaption("游戏名");
    info.setContent("历史最高分");
    n_y = info.infoBar(n_y);

    for (int i = 0; i < GAME_NUM; i++)
    {
        if (strcmp(game.gameNames[i], "") != 0)
        {
            info.setCaption(game.gameNames[i]);
            info.setContent(to_string(game.bestScores[i]));
            n_y = info.infoBar(n_y);
        }
    }
    return ++n_y;
}

#endif