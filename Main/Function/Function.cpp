#include "Function.h"
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
using namespace std;

//限制只能输入数字
int getMenuChoice()
{
    int choice;

    while (true)
    {
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "输入错误，请输入数字：";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return choice;
    }
}

//时间转换函数
//将时间戳转成年月日时分秒string类型
string timeToString(time_t t)
{
    if (t == 0)
    {
        return "未记录";//初始化和撤销叫号，endtime都会赋值为 0
    }
    tm* localTime = localtime(&t);//tm 结构体
    stringstream ss;
    ss << put_time(localTime, "%Y-%m-%d %H:%M:%S");//拼接成一个 string 类型
    return ss.str();
}
