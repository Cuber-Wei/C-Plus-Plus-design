#ifndef _LOGIN_H_
#define _LOGIN_H_

#include <iostream>
#include <fstream>
#include <cstring>

#include "Moduel.h"
#include "CryptoUtils.h"
#include "Utils.h"
#include "Person.h"
#include "MenuUI.h"
using namespace std;

class LogIn : public Moduel
{
protected:
    string path = "./data"; // uid存储路径
    int uid = 0;            // uid初始化
    char password[100] = "";
    Account acc;
    Person user; //账号信息

public:
    LogIn();                         //构造函数
    int menu(int start_y, int mode); //菜单
    bool check(int mode);            //校验函数
    Person processControl();         //流程控制
};
LogIn::LogIn() { name = "账号登录模块"; }
int LogIn::menu(int start_y, int m = 0)
{
    int n_y = start_y;
    Menu info("模块", name, LINE_WIDTH, 100, 50);
    n_y = info.infoBar(n_y);
    if (m == 0) //选择登入账户类型
    {
        string title = "选择账号类型";
        string items[2] = {
            "1. 管理员(admin)",
            "2. 普通用户"};
        Menu menu0(title, items, LINE_WIDTH, 2, 100, 50);
        return menu0.menuUI(n_y);
    }
    return n_y;
}
bool LogIn::check(int mode = 1) //区分管理员账号及普通用户账号
{
    bool res = false;
    fstream dataFile;
    if (mode == 1) //管理员账号 ----- uid:1, password:admin
        return strcmp(password, "admin") == 0 && uid == 1;
    else
    {
        CryptoUtils enc;
        strcpy(this->password, enc.RSAUtil(password, '1'));
        dataFile.open("./data/" + to_string(uid) + "/" + to_string(uid) + ".dat", ios::in | ios::binary);
        if (dataFile.fail())
        {
            system("cls");
            Menu info("", "文件打开失败!", LINE_WIDTH, 100, 50);
            info.infoBar(3, 3);
        }
        dataFile.read((char *)&acc, sizeof(Account));
        if (strcmp(this->password, acc.password) == 0) //密码正确
            res = true;
        dataFile.close();
    }
    return res;
}
Person LogIn::processControl()
{
    system("cls");
    int choice = 0, ch;
    choice = menu(1);
    //输入相关参数
    system("cls");
    int n_y = 3;
    Menu input("输入uid:", "", LINE_WIDTH, 100, 50);
    char buf[20];
    n_y = input.input(n_y, buf);
    for (int i = 0; buf[i] != '\0'; i++)
        uid = uid * 10 + buf[i] - '0';
    user.setUID(uid);

    input.setCaption("输入密码:");
    n_y = input.input(n_y, password, 20, true);
    //重新输入直到成功或者放弃登录.
    while (!check(choice))
    {
        system("cls");
        string title = "放弃登录?";
        string items[1] = {
            "是否放弃登录?[Y/n]",
        };
        Menu menu2(title, items, LINE_WIDTH, 1, 100, 50);
        ch = menu2.menuUI(1, 2);
        if ((ch + '0') == 'Y' || (ch + '0') == 'y')
        {
            user.setUID(0);
            return user; //未登录成功
        }

        system("cls");
        int n_y = 3;
        input.setCaption("输入uid:");
        n_y = input.input(n_y, buf);
        uid = 0;
        for (int i = 0; buf[i] != '\0'; i++)
            uid = uid * 10 + buf[i] - '0';
        user.setUID(uid);

        input.setCaption("输入密码:");
        n_y = input.input(n_y, password, 20, true);
    }
    if (choice == 1)
    {
        strcpy(acc.name, "admin");
        user.setUID(10000000);
        user.updateAccData(acc);
        return user;
    }
    Person res(uid); //通过uid构造Person对象
    system("cls");
    input.setContent("登录成功!");
    input.setCaption("");
    input.infoBar(3, 3);
    return res;
}

#endif