#ifndef _MENUUI_H_
#define _MENUUI_H_
#include <iostream>
#include <cstring>
#include <conio.h>
#include <windows.h>
#include "Utils.h"
using namespace std;
#define LINE_WIDTH 50 //行宽50
//菜单UI界面整合
class Menu
{
private:
    string title, caption, content;
    string items[10];
    int midWidth, width, num;
    int Width = 100, Height = 50;
    void output_board(int i, int j, int chang, int kuan);
    int getChoice(int num, int mode); // mode:1->数字;  2:Y/n

public:
    Menu(string title, string items[], int width, int num, int W, int H); //构造函数1
    Menu(string caption, string content, int width, int W, int H);        //构造函数2

    void setCaption(string caption) { this->caption = caption; }
    void setContent(string content) { this->content = content; }

    void gotoxy(int x, int y);
    int color(int c);
    void gotoMid(int y, int length);

    int menuUI(int start, int mode);                                    //菜单展示 mode:1->数字;  2:Y/n
    int infoBar(int start_y, int mode);                                 //信息展示栏
    int input(int start_y, char *buf, int maxlength, bool isPassInput); //输入框
};
Menu::Menu(string caption, string content, int width, int W, int H)
{
    this->caption = caption;
    this->content = content;
    this->width = width;
    this->Width = W;
    this->Height = H;
    this->midWidth = Width / 2 - width / 2;
}
Menu::Menu(string title, string items[], int width, int num, int W, int H)
{
    this->num = num;
    this->title = title;
    for (int i = 0; i < num; i++)
        this->items[i] = items[i];
    this->width = width;
    this->Width = W;
    this->Height = H;
    this->midWidth = Width / 2 - width / 2;
}
int Menu::getChoice(int num, int mode = 1)
{
    int count = 0;
    char c = '\0', res = '\0';
    while ((c = getch()) != '\r' || res == '\0' || (mode == 1 && !(res >= '1' && res <= '0' + num)) || (mode == 0 && !(res == 'Y' || res == 'y' || res == 'N' || res == 'n'))) //直到结束输入
    {
        if (c == 8) // 退格
        {
            if (count == 0)
            {
                res = '\0';
                continue;
            }
            putchar('\b'); // 回退一格
            putchar(' ');
            putchar('\b'); // 再回退一格等待输入
            count--;
            continue;
        }
        if (count == 1)
            continue;
        if (c != '\r')
        {
            putchar(c);
            res = c;
            count++;
        }
    }
    return res - '0';
}
void Menu::gotoxy(int x, int y)
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
int Menu::color(int c)
{
    // SetConsoleTextAttribute是API设置控制台窗口字体颜色和背景色的函数
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); //更改文字颜色
    return 0;
}
void Menu::gotoMid(int y, int length)
{
    int mid = Width / 2 - length / 2;
    gotoxy(mid, y);
}
void Menu::output_board(int i, int j, int end_x, int end_y)
{
    //(i,j)表示边框的左上角
    // i代表第几行,j代表第几列
    int last_line = end_y;
    int last_col = end_x;
    int first_j = j;
    int first_i = i;
    for (; i <= last_line; i++)
    {
        //先遍历行
        j = first_j;
        for (; j <= last_col; j++)
        {
            //再遍历列
            gotoxy(j, i);
            if (i == first_i || i == last_line)
            {
                printf("-");
            }
            else if (j == first_j || j == last_col)
            {
                printf("|");
            }
        }
    }
    gotoxy(0, 0); //控制台输出位置归零
}
int Menu::menuUI(int start, int mode = 1)
{
    //计算补全位数
    int k = 0;
    gotoxy(midWidth, 3 + start);
    //淡蓝色，居中输出标题
    color(11);
    int pad = (width - 2 - title.length()) / 2;
    for (int k = 0; k < pad; k++)
        putchar(' ');
    cout << title;

    //绿色,输出游戏选项
    color(10);
    int n_y = 7 + start, length;
    for (int k = 0; k < num; k++)
    {
        gotoxy(midWidth + 1, n_y + 1);
        length = items[k].length();
        pad = (width - length - 2) / 2;
        for (int p = 0; p < pad; p++)
            putchar(' ');
        cout << items[k];
        n_y += 2;
    }

    //白色，输出边框
    color(14);
    output_board(6 + start, midWidth, midWidth + width - 1, n_y + 1);

    gotoxy(midWidth, n_y + 5);
    //蓝色，输出游戏选项选择字体
    color(3);
    int pos = 0;
    if (mode == 1)
    {
        pos = midWidth + 10 + num * 2;
        cout << "请选择:[";
        for (int k = 1; k <= num; k++)
        {
            if (k != num)
                cout << k << " ";
            else
                cout << k << "]";
        }
        cout << ":[ ]";
    }
    else
    {
        pos = midWidth + 14;
        cout << "请选择:[Y/n]:[ ]";
    }

    //作者署名
    color(7);
    gotoxy(Width - 20, Height - 3);
    cout << "by 162210118";
    gotoxy(Width - 20, Height - 2);
    cout << "魏程浩";

    //获取选择
    gotoxy(pos, n_y + 5);
    return getChoice(num, mode);
}
int Menu::infoBar(int start_y, int mode = 1)
{
    //计算补全位数
    int n_y = start_y, j = 0;
    if (mode == 1) //单行展示数据
    {              //对caption, content格式化整合后的字符串，长度为LINE_WIDTH
        string line;
        int lenOfCap = caption.length();
        int lenOfCon = content.length();
        line = caption;
        for (int p = lenOfCap; p < width - lenOfCap - lenOfCon - 2; p++)
            line += ' ';
        line += content;
        gotoxy(midWidth + 1, start_y);
        //淡蓝色，居中靠左输出标题
        color(11);
        cout << line;
        n_y++;
    }
    else if (mode == 2) //长语句,自动分行
    {
        string lines[10];
        for (int i = 0; i < content.length(); i++)
        {
            if (i % (9 * width / 10) == 0 && i != 0)
                j++;
            lines[j] += content[i];
        }
        gotoxy(midWidth + 1, start_y);
        //淡蓝色，居中靠左输出
        color(11);
        for (int i = 0; i <= j; i++)
        {
            cout << lines[i];
            n_y++;
            gotoxy(midWidth + 1, n_y);
        }
    }
    else
    {
        gotoxy(midWidth, start_y);
        //淡蓝色，居中输出标题
        color(11);
        int pad = (width - 2 - content.length()) / 2;
        for (int k = 0; k < pad; k++)
            putchar(' ');
        cout << content;
        n_y++;
    }
    //白色，输出边框
    color(14);
    output_board(start_y - 1, midWidth, midWidth + width - 1, n_y++);
    //恢复白色
    color(7);
    return n_y;
}
int Menu::input(int start_y, char *buff, int maxlength = 20, bool isPassInput = false)
{
    int n_y = start_y, j = 0;
    gotoxy(midWidth + 1, start_y);
    //淡蓝色，居中靠左输出标题
    color(11);
    cout << caption;
    n_y += 2;
    //白色，输出边框
    color(14);
    output_board(start_y - 1, midWidth, midWidth + width - 1, n_y++);
    //恢复白色
    color(7);
    gotoxy(midWidth + 1, n_y - 2);
    if (!isPassInput)
        cin >> buff;
    else
        inputPassword(buff, maxlength);
    return n_y;
}

#endif