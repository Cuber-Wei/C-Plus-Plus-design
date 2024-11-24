#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <cstring>
#include <windows.h>
using namespace std;
#define PASS_MAXSIZE 20 //密码规定长度小于16
#define LINE_WIDTH 50   //行宽50

//密码隐式输入函数
void inputPassword(char *str, int size = PASS_MAXSIZE)
{
    /*
    str:    缓存区
    size:   最大长度
    */
    char c;
    int count = 0;
    char *password = new char[size]; // 动态申请空间

    while ((c = getch()) != '\r') //直到结束输入
    {
        if (c == 8) // 退格
        {
            if (count == 0)
                continue;
            putchar('\b'); // 回退一格
            putchar(' ');  // 输出一个空格将原来的*隐藏
            putchar('\b'); // 再回退一格等待输入
            count--;
            continue;
        }
        if (count == size - 1) // 最大长度为size-1
            continue;
        putchar('*'); // 接收到一个字符后, 打印一个*
        password[count] = c;
        count++;
    }
    password[count] = '\0';
    strcpy(str, password);
    delete[] password; // 释放空间
    cout << endl;
}
#endif