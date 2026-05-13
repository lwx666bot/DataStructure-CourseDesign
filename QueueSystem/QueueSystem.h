#ifndef QUEUESYSTEM_H
#define QUEUESYSTEM_H
#include <iostream>
#include <string>
#include<vector>
#include <stack>
#include "../Customer/Customer.h"

using namespace std;
const int MaxSize=100;
class QueueSystem{//循环队列类
    private:
    Customer* CustomerQueue;//储存队列
    int front;//队头，队非空时，指向队头前一个元素，实际存储空间 MaxSize-1
    int rear;//队尾，队非空时，指向队尾元素
    int length;//队列长度
    stack<Customer> stk;//栈用来处理撤销叫号，同时表示已经完成的顾客
    /*vector<Customer>historyRecords
    历史信息统计，
    包括所有曾经入队顾客，
    顾客结构体中用 status 记录*/
    vector<Customer>historyRecords;
    int AutoCustomerID;//给顾客ID赋值用  
    int AutoQueueNumber;//排队序号数
    string historyFileName;//历史信息记录文件名称
    string createHistoryFileName();//生成本次运行程序的历史信息记录文件名称
    public:
    //构造函数，初始化队列
    //historyFileName=createHistoryFileName();用本次开始运行的时间作为记录
    QueueSystem(){
        CustomerQueue=new Customer [MaxSize];
        length=front=rear=0;
        AutoCustomerID=1001;
        AutoQueueNumber=1;
        historyFileName=createHistoryFileName();
    }
    ~QueueSystem(){
        cout<<"系统已销毁";
        delete []CustomerQueue;
    }
    bool empty();//队空判断
    int Getlength();//求队列长度
    bool push(Customer& e);//入队
    void addCustomer();//添加顾客入队
    bool pop();//队头出队
    bool Getfront(Customer&e);//取队头
    void callCustomer();//叫号
    void DispQueue();//查看当前排队队列
    void CancelCall();//撤销叫号
    void ClearQueue();//清空等待队列
    void DispHistory();//历史信息查看
    void showSystem();//系统信息
    void SaveHistoryToFile();//保存信息到文件中
    bool hasHistoryRecords();//判断是否有队列信息产生
};
#endif
