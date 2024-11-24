#ifndef _DATA_H_
#define _DATA_H_
#define GAME_NUM 4
struct Account
{
    char name[10] = "";      //用户名
    char password[100] = ""; //密码(加密后)
};
struct GameInfo
{
    char gameNames[GAME_NUM][10] = {""}; //玩过的游戏名
    int bestScores[GAME_NUM] = {0};      //历史最高得分
};
#endif