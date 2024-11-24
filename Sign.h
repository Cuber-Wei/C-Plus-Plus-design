#ifndef _SIGN_H_
#define _SIGN_H_
#include "Moduel.h"
#include "CryptoUtils.h"
#include "Utils.h"
#include "Person.h"
#include "MenuUI.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>
using namespace std;

class Sign : public Moduel
{
protected:
    string path = "./data";      // uid存储路径
    int uid = 100000000;         // uid初始化
    char passConfirm[100] = "*"; //初始化
    Account acc;                 //账号信息
    Person user;                 //用户对象

public:
    Sign();                          //构造函数
    int menu(int start_y, int mode); //菜单
    void updateUID();                //获取uid
    void processControl();           //流程控制
};

Sign::Sign() { name = "账号注册模块"; }
int Sign::menu(int start_y, int mode = 0)
{
    int n_y = start_y;
    Menu info("模块", name, LINE_WIDTH, 100, 50);
    n_y = info.infoBar(n_y);
    if (mode == 0)
    {
        info.setCaption("输入要注册的账号昵称:");
        info.setContent("");
        n_y = info.input(n_y, acc.name);
    }
    return n_y;
}
void Sign::updateUID()
{
    fstream uidFile;
    int uidInt = 0;
    uidFile.open(path + "/uid.dat", ios::in | ios::out | ios::binary);
    if (uidFile.fail())
    {
        system("cls");
        Menu info("", "密码文件打开失败!", LINE_WIDTH, 100, 50);
        info.infoBar(3, 3);
        return;
    }
    uidFile.read((char *)&uid, sizeof(int)); //获取当前uid
    //更新uid
    uid++;
    uidFile.seekg(0, ios::beg);
    uidFile.write((char *)&uid, sizeof(int));
    uidFile.close();
}
void Sign::processControl()
{
    int n_y = 3;
    char ch;
    updateUID();
    //菜单打印及注册账号输入
    system("cls");
    n_y = menu(n_y, 0);
    //校验密码
    //直到用户退出或者两次输入相匹配
    while (strcmp(passConfirm, acc.password) != 0)
    {
        n_y = 3;
        n_y = menu(n_y, 1);
        system("cls");
        Menu info("正在注册账号 ", acc.name, LINE_WIDTH, 100, 50);
        n_y = info.infoBar(n_y);
        Menu inputPass("输入密码:", "", LINE_WIDTH, 100, 50);
        n_y = inputPass.input(n_y, acc.password, PASS_MAXSIZE, true);
        inputPass.setCaption("确认密码:");
        n_y = inputPass.input(n_y, passConfirm, PASS_MAXSIZE, true);

        if (strcmp(passConfirm, acc.password) != 0)
        {
            n_y = 1;
            system("cls");
            Menu warn("二次密码输入不同!请重新输入!", "", LINE_WIDTH, 100, 50);
            n_y = warn.infoBar(n_y);
            string title1 = "退出?";
            string items1[1] = {
                "是否退出注册程序?[y/n]",
            };
            Menu menu2(title1, items1, LINE_WIDTH, 1, 100, 50);
            ch = menu2.menuUI(n_y, 0);
            if ((ch + '0') == 'Y' || (ch + '0') == 'y')
                return;
        }
    }
    //保存账户数据
    //创建文件夹
    path = path + "/" + to_string(uid);
    CreateDirectory(path.c_str(), NULL);
    path += "/game_save";
    CreateDirectory(path.c_str(), NULL);
    //账号信息加密(密码RSA加密)
    CryptoUtils enc;
    strcpy(acc.password, enc.RSAUtil(acc.password, '1'));
    //保存数据
    user.setUID(uid);
    user.updateAccData(acc);
    system("cls");
    menu(1, 1);

    system("cls");
    Menu info("", "", LINE_WIDTH, 100, 50);
    string content = "账户";
    content += acc.name;
    content += " 注册成功！";
    info.setContent(content);
    int y = info.infoBar(3, 3);

    content = "您的uid为";
    content += to_string(uid);
    content += ",请牢记!";
    info.setContent(content);
    info.infoBar(y, 3);
    system("pause");
}

#endif