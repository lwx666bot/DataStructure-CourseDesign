#ifndef QUEUESYSTEM_H
#define QUEUESYSTEM_H
#include<stack>
#include"Customer.h"
using namespace std;
const int MaxSize=100;
class QueueSystem{//循环队列类
    private:
    Customer* CustomerQueue;//储存队列
    int front;//队头，队非空时，指向队头前一个元素
    int rear;//队尾，队非空时，指向队尾元素
    int length;//队列长度
    int nextCustomerID;//给顾客ID赋值用
    int nextQueueNumber;//叫号数
    stack<Customer> stk;
    public:
    QueueSystem(){
        CustomerQueue=new Customer [MaxSize];
        length=front=rear=0;
        nextCustomerID=1001;
        nextQueueNumber=1;
    }
    ~QueueSystem(){delete []CustomerQueue;}
    bool empty();//队空判断
    int Getlength();//求队列长度
    bool push(Customer& e);//入队
    void addCustomer();//添加顾客入队
    bool pop();//队头出队
    bool Getfront(Customer&e);//取队头
    void callCustomer();//叫号
    void DispQueue();//查看当前排队队列
    void CancelCall();
};
#endif