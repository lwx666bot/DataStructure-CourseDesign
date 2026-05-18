#include <iostream>
#include <cstdlib>
#include "Menu.h"
#include "../Function/Function.h"
#include "../../RestaurantQueueSystem/QueueSystem/QueueSystem.h"
#include "../../ExpressionCalculatorSystem/ExpressionCalculator/ExpressionCalculator.h"
using namespace std;    

//用于暂停终端，显示信息
void pauseScreen(){
    cout<<endl;
    system("pause");
}
void queueMenu(QueueSystem&qu);
void calculatorMenu(ExpressionCalculator& expr);

void printCalculatorMenu()
{
    system("cls");//清空终端
    cout << endl;
    cout << "========================================" << endl;
    cout << "             表达式计算器系统" << endl;
    cout << "========================================" << endl;
    cout << "1. 输入中缀表达式" << endl;
    cout << "2. 求前缀和后缀表达式" << endl;
    cout << "3. 基于中缀和后缀表达式求值" << endl;
    cout << "4. 基于表达式树求值" << endl;
    cout << "5. 表达式树可视化" << endl;
    cout << "6. 显示表达式结果" << endl;
    cout << "7. 保存表达式结果到文件" << endl;
    cout << "8. 查看计算历史" << endl;
    cout << "9. 清空计算历史" << endl;
    cout << "10. 清空当前表达式" << endl;
    cout << "11. 算法功能演示" << endl;
    cout << "0. 返回主菜单" << endl;
    cout << "========================================" << endl;
    cout << "请输入你的选择：";
}

void printQueueMenu()
{
    system("cls");//清空终端
    cout << endl;
    cout << "========================================" << endl;
    cout << "              排队叫号系统" << endl;
    cout << "========================================" << endl;
    cout << "1. 顾客取号排队" << endl;
    cout << "2. 窗口叫号服务" << endl;
    cout << "3. 查看当前排队情况" << endl;
    cout << "4. 撤销叫号" << endl;
    cout << "5. 清空队列" << endl;
    cout << "6. 查看历史服务记录" << endl;
    cout << "7. 查看系统状态" << endl;
    cout << "8. 存储信息到文本文件中" << endl;
    cout << "0. 返回主菜单" << endl;
    cout << "========================================" << endl;
    cout << "请输入你的选择：";
}

void printMainMenu(){
    system("cls");//清空终端
    cout<<"=====数据结构课程设计系统====="<<endl;
          cout<<"1,校园餐厅排队叫号模拟系统" <<endl;
         cout<< "2,表达式计算器"<<endl;
          cout<<"0,退出系统"<<endl;
          cout<<"请输入选项"<<endl;
}

void mainMenu(){
     int choice;
     QueueSystem qu;
    ExpressionCalculator expr;
    while(true){
        printMainMenu();
        choice = getMenuChoice();
        switch(choice){
            case 0:{//这里自动保存退出系统时，队列已经存储的信息
                if(qu.hasHistoryRecords()){
                    qu.SaveHistoryToFile();
                    cout<<"退出系统前，信息已自动保存"<<endl;
                }else{
                    cout<<"本次运行无排队历史记录，无需保存"<<endl;
                }
                if(expr.hasHistoryRecords()){
                    expr.SaveResultToFile();
                    cout<<"退出系统前，表达式计算历史已自动保存"<<endl;
                }else{
                    cout<<"本次运行无表达式计算历史记录，无需保存"<<endl;
                }
                cout<<"退出系统...";
                return;
                break;
            }
            case 1:{
                //进入队列叫号二级菜单，进行排队模拟相关操作
                queueMenu(qu);
                break;
            }
            case 2:{
                //进入表达式计算器二级菜单，进行表达式计算器相关操作
                calculatorMenu(expr);
                break;
            }
            default:{
                cout<<"无效输入，重新选择。。。"<<endl;break;
            }
            
        }
    }
}

void queueMenu(QueueSystem&qu)
{
    int queuechoice;

    while (true)
    {
        printQueueMenu();
        queuechoice = getMenuChoice();

        switch (queuechoice)
        {
            case 0:
            {
                cout << "返回主菜单..." << endl;
                return;
            }

            case 1:
            {
                // 顾客取号排队
                qu.addCustomer();
                pauseScreen();
                break;
            }

            case 2:
            {
                // 窗口叫号服务
                qu.callCustomer();
                pauseScreen();
                break;
            }

            case 3:
            {
                // 查看当前排队情况
                qu.DispQueue();
                pauseScreen();
                break;
            }

            case 4:
            {
                // 撤销叫号
                qu.CancelCall();
                pauseScreen();
                break;
            }

            case 5:
            {
                // 清空等待队列
                qu.ClearQueue();
                pauseScreen();
                break;
            }

            case 6:
            {
                // 查看历史服务记录
                qu.DispHistory();
                pauseScreen();
                break;
            }
            case 7:
            {
                // 查看系统状态
                qu.showSystem();
                pauseScreen();
                break;
            }

            case 8:
            {
                //记录数据到文件中
                qu.SaveHistoryToFile();
                pauseScreen();
                break;
            }

            default:
            {
                cout << "无效输入，请重新选择..." << endl;
                break;
            }
        }
    }
}

void calculatorMenu(ExpressionCalculator &expr)
{
    int calculatorchoice;

    while (true)
    {
        printCalculatorMenu();
        calculatorchoice = getMenuChoice();

        switch (calculatorchoice)
        {
            case 0:
            {
                cout << "返回主菜单..." << endl;
                return;
            }

            case 1:
            {
                // 输入中缀表达式
                expr.InputExpression();
                pauseScreen();
                break;
            }

            case 2:
            {
                // 求前缀和后缀表达式
                expr.InfixToPost();
                expr.InfixToPre();
                pauseScreen();
                break;
            }

            case 3:
            {
                // 基于中缀和后缀表达式求值
                expr.GetInfixValue();
                expr.GetPostValue();
                pauseScreen();
                break;
            }

            case 4:
            {
                // 基于表达式树求值
                expr.GetTreeValue();
                pauseScreen();
                break;
            }

            case 5:
            {
                // 表达式树可视化
                expr.DispExpressionTree();
                pauseScreen();
                break;
            }

            case 6:
            {
                // 显示表达式结果
                expr.DispResult();
                pauseScreen();
                break;
            }

            case 7:
            {
                // 保存表达式结果到文件
                expr.SaveResultToFile();
                pauseScreen();
                break;
            }

            case 8:
            {
                // 查看计算历史
                expr.DispHistory();
                pauseScreen();
                break;
            }

            case 9:
            {
                // 清空计算历史
                expr.ClearHistory();
                pauseScreen();
                break;
            }

            case 10:
            {
                // 清空当前表达式
                expr.ClearExpression();
                pauseScreen();
                break;
            }

            case 11:
            {
                // 算法功能演示
                expr.ShowAlgorithm();
                pauseScreen();
                break;
            }

            default:
            {
                cout << "无效输入，请重新选择..." << endl;
                break;
            }
        }
    }
}
