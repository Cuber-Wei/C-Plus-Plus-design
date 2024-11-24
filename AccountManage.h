#ifndef _MODULE_H_
#define _MODULE_H_

#include <iostream>
#include <fstream>
#include <cstring>

#include "Moduel.h"
#include "CryptoUtils.h"
#include "Utils.h"
#include "Person.h"
#include "MenuUI.h"

using namespace std;

class AccountManage : public Moduel
{
protected:
    string path = "./data"; // uid存储路径
    int uid = 0;            // uid初始化
    Account newAcc;         //新账户信息
    Person *user;           //账号信息
    bool isAdmin = false;   //管理员登录

public:
    AccountManage(Person &obj);              //构造函数
    int menu(int start_y, int mode);         //菜单
    int printInfo(int n_y);                  //展示用户信息
    void updateInfo(int start);              //修改账号
    void resetAcc(int start);                //重置账号信息(重置密码)
    void deleteAcc(int start, int uidIn);    //注销账号
    void checkInfo(int start);               //查看账号信息
    bool isIn(char goal[10], char from[10]); //模糊查找
    void processControl();                   //流程控制
};

AccountManage::AccountManage(Person &obj)
{
    name = "账号管理模块";
    uid = obj.getUID();
    user = &obj;
    if (obj.getUID() == 10000000)
        isAdmin = true;
}
int AccountManage::menu(int start_y, int mode = 0)
{
    int n_y = start_y;
    Menu info("模块", name, LINE_WIDTH, 100, 50);
    n_y = info.infoBar(n_y);
    switch (mode)
    {
    case 0:
    {
        string title = "选择功能";
        string items[3] = {
            "1.修改账号信息",
            "2.注销账号",
            "3.查找账号",
        };
        Menu menu0(title, items, LINE_WIDTH, 3, 100, 50);
        return menu0.menuUI(n_y);
    }
    case 1:
        system("cls");
        n_y = 3;
        n_y = printInfo(n_y);
        info.setCaption("功能");
        info.setContent("修改账号信息");
        n_y = info.infoBar(n_y);
        updateInfo(n_y);
        break;
    case 2:
        system("cls");
        n_y = 3;
        n_y = printInfo(n_y);
        info.setCaption("功能");
        info.setContent("注销账号");
        n_y = info.infoBar(n_y);
        deleteAcc(n_y, user->getUID());
        break;
    case 3:
        system("cls");
        n_y = 3;
        n_y = printInfo(n_y);
        info.setCaption("功能");
        info.setContent("查找账号");
        n_y = info.infoBar(n_y);
        checkInfo(n_y);
        break;
    case 4:
    {
        string title = "管理员菜单:";
        string items[2] = {
            "1.重置账号密码",
            "2.查看账户信息",
        };
        Menu menu1(title, items, LINE_WIDTH, 2, 100, 50);
        return menu1.menuUI(n_y);
    }
    case 5:
        system("cls");
        n_y = 3;
        n_y = printInfo(n_y);
        resetAcc(n_y);
        break;
    case 6:
        system("cls");
        n_y = 3;
        n_y = printInfo(n_y);
        checkInfo(n_y);
        break;
    }
    return -1;
}
void AccountManage::updateInfo(int start) //修改账号
{
    Menu input("", "输入新账号信息:", LINE_WIDTH, 100, 50);
    int n_y = input.infoBar(start, 3);
    input.setCaption("输入新账号昵称:");
    n_y = input.input(n_y, newAcc.name);
    input.setCaption("输入新账号密码:");
    n_y = input.input(n_y, newAcc.password, 20, true);
    CryptoUtils enc;
    strcpy(newAcc.password, enc.RSAUtil(newAcc.password, '1'));
    user->updateAccData(newAcc);
}
void AccountManage::resetAcc(int start)
{
    Menu info("", "", LINE_WIDTH, 100, 50);
    Menu input("", "", LINE_WIDTH, 100, 50);
    int goalUid = 0, n_y = start;
    if (!isAdmin)
    {
        info.setContent("您未持有足够权限!");
        info.infoBar(n_y, 3);
        system("pause");
        return;
    }
    checkInfo(n_y);
    input.setCaption("输入要重置密码的账号的uid:");
    char buf[20];
    n_y = input.input(n_y, buf);
    for (int i = 0; buf[i] != '\0'; i++)
        goalUid = goalUid * 10 + buf[i] - '0';

    Person *tmp = new Person(goalUid);

    string title = "确认要重置的账号信息";
    string uid = to_string(tmp->getUID());
    string name = tmp->getName();
    uid = "uid: " + uid;
    name = "昵称: " + name;
    string items[2] = {
        uid,
        name};
    Menu menu0(title, items, LINE_WIDTH, 2, 100, 50);
    int ch = menu0.menuUI(n_y, 0);
    if ((ch + '0') == 'Y' || (ch + '0') == 'y')
    {
        strcpy(newAcc.name, tmp->getName());
        strcpy(newAcc.password, "12345678");
        CryptoUtils enc;
        strcpy(newAcc.password, enc.RSAUtil(newAcc.password, '1'));
        tmp->updateAccData(newAcc);
        info.setContent("重置成功!");
        system("cls");
        info.infoBar(3, 3);
    }
}
void AccountManage::deleteAcc(int start, int uidIn = -1) //注销账号
{
    int ch, n_y = start;
    string title = "确定注销账号";
    string items[1] = {
        "确定注销账号" + to_string(uidIn) + "吗?[Y/n]"};
    Menu menu0(title, items, LINE_WIDTH, 1, 100, 50);
    Menu info("", "", LINE_WIDTH, 100, 50);
    ch = menu0.menuUI(n_y, 0);
    if ((ch + '0') == 'Y' || (ch + '0') == 'y')
    {
        remove(("./data/" + to_string(uidIn)).c_str());
        info.setContent("账号" + to_string(uidIn) + "注销成功!");
        info.infoBar(n_y, 3);
    }
}
bool AccountManage::isIn(char goal[10], char from[10])
{
    char *p = from, *q = goal;
    bool flag = false;
    for (; *(p + strlen(goal) - 1); p++)
    {
        for (q = goal; *p == *q && *q; p++, q++)
            ;
        if (!*q)
        {
            flag = true;
            break;
        }
    }
    return flag;
}
void AccountManage::checkInfo(int start) //查看账号信息
{
    int ch, uidIn = 0, n_y = start;
    string title = "选择查找帐号的方式";
    string items[2] = {
        "1.uid查找",
        "2.昵称查找"};
    Menu menu0(title, items, LINE_WIDTH, 2, 100, 50);
    Menu input("输入uid:", "", LINE_WIDTH, 100, 50);
    Menu info("", "", LINE_WIDTH, 100, 50);
    ch = menu0.menuUI(n_y);

    if (ch == 1) //精确查找,只有一条记录
    {
        system("cls");
        n_y = 3;
        char buf[20];
        n_y = input.input(n_y, buf);
        for (int i = 0; buf[i] != '\0'; i++)
            uidIn = uidIn * 10 + buf[i] - '0';
        Person tmp(uidIn, 2);
        if (tmp.getUID() != -1)
        {
            n_y = tmp.showInfo(n_y);
            n_y = tmp.showGamed(n_y);
        }
        system("pause");
    }
    else if (ch == 2)
    {
        system("cls");
        n_y = 3;
        input.setCaption("输入昵称:");
        input.input(n_y, newAcc.name);

        fstream uidFile;
        int uidInt = 0;
        uidFile.open(path + "/uid.dat", ios::in | ios::binary);
        if (uidFile.fail())
        {
            info.setContent("uid文件打开失败! 文件可能不存在.");
            info.infoBar(n_y, 3);
            return;
        }
        uidFile.read((char *)&uidInt, sizeof(int)); //获取当前uid
        uidFile.close();

        system("cls");
        string caption = "昵称中包含";
        caption += newAcc.name;
        caption += "的有:";
        info.setCaption(caption);
        info.infoBar(n_y);

        int cnt = 0, page = 0;
        for (int i = 10000000; i <= uidInt; i++)
        {
            Person *tmp = new Person(i, 1); //搜索模式,文件打开不报错
            if (isIn(newAcc.name, tmp->getName()))
            {
                n_y = tmp->showInfo(n_y);
                n_y = tmp->showGamed(n_y);
                cnt++;
            }
            delete tmp;
            if (cnt % 4 == 0 && cnt != 0) //四条记录为一页
            {
                page++;
                cnt = 0;
                string content = "第";
                content += to_string(page);
                content += "页";
                info.setContent(content);
                info.infoBar(n_y, 3);
                system("pause");

                n_y = 3;
                system("cls");
                string caption = "昵称中包含";
                caption += newAcc.name;
                caption += "的有:";
                info.setCaption(caption);
                info.infoBar(n_y);
            }
            else if (i == uidInt) //枚举到最后一条,且目前页面不足四条
            {
                page++;
                string content = "第";
                content += to_string(page);
                content += "页";
                info.setContent(content);
                info.infoBar(n_y, 3);
                system("pause");
            }
        }
    }
}
int AccountManage::printInfo(int n_y)
{
    if (isAdmin)
    {
        Menu info("", "管理员", LINE_WIDTH, 100, 50);
        n_y = info.infoBar(3, 3);
        return n_y;
    }
    n_y = user->showInfo(n_y);
    n_y = user->showGamed(n_y);
    return n_y;
}
void AccountManage::processControl()
{
    Menu info("", "请先登录!", LINE_WIDTH, 100, 50);
    //检验是否登录过(uid == 0)
    if (user->getUID() == 0)
    {
        system("cls");
        info.infoBar(3, 3);
        system("pause");
        return;
    }
    int ch = 0;
    int n_y = 3;

    system("cls");

    if (isAdmin) //管理员操作
    {
        ch = 4;
        string content = "管理员";
        info.setContent(content);
        n_y = info.infoBar(3, 3);
    }
    else
        n_y = printInfo(n_y);

    ch = menu(n_y, ch);
    menu(n_y, ch + 4 * isAdmin);
    system("pause");
}

#endif