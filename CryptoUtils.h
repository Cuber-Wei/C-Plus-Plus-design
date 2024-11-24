#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#include <iostream>
#include <fstream>
#include <cstdio>
using namespace std;
class CryptoUtils
{
public:
    char *RSAUtil(char str[100], char mod = '1')
    {
        fstream dataFile;
        int i = 0;
        dataFile.open("input.txt", ios::out);
        // 模式输入
        dataFile.put(mod);
        dataFile.put(',');
        // 数据输入
        while (str[i] != '\0')
        {
            dataFile.put(str[i]);
            i++;
        }
        dataFile.close();
        // 脚本执行
        system("crypto.exe");
        // 读取输出
        dataFile.open("output.txt", ios::in);
        dataFile.getline(str, 100);
        dataFile.close();
        // 删除中间数据文件
        remove("input.txt");
        remove("output.txt");
        return str;
    }
};

#endif