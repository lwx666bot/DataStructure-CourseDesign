#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "QueueSystem.h"
using namespace std;
//时间转换函数
string timeToString(time_t t)
{
    if (t == 0)
    {
        return "未记录";
    }
    tm* localTime = localtime(&t);
    stringstream ss;
    ss << put_time(localTime, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

bool QueueSystem::empty(){
    return front==rear;
}

int QueueSystem::Getlength(){
    return length;
}

bool QueueSystem::push(Customer& e){
    if((rear+1)%MaxSize==front)
        return false;
    rear=(rear+1)%MaxSize;
    CustomerQueue[rear]=e;
    length++;
    return true;
}

void QueueSystem:: addCustomer(){
    string str;
    cout<<"请输入顾客姓名(输入-1取消本次叫号排队)"<<endl;
    cin>>str;
    if(str=="-1")return;
    Customer a(nextCustomerID,nextQueueNumber,str);
    bool flag=push(a);
    nextCustomerID++;
    nextQueueNumber++;
    if(flag){
        cout<<"入队成功"<<endl;
        cout<<"姓名："<<a.name<<endl;
        cout<<"ID："<<a.customerID<<endl;
        cout<<"序号："<<a.queueNumber<<endl;
        cout<<"入队时间："<<timeToString(a.arrivetime);//🤣
    }
    else cout<<"入队失败";
}

bool QueueSystem::pop(){
    if(empty())return false;
    front=(front+1)%MaxSize;
    length--;
    return true;
}

bool QueueSystem::Getfront(Customer&e){
    if(empty())return false;
    int front1=(front+1)%MaxSize;
    e=CustomerQueue[front1];
    return true;
}

void QueueSystem::callCustomer(){
    if(empty()){
        cout<<"队空，暂无顾客"<<endl;
        return;
    }
    Customer a;
    Getfront(a);
    pop();
    a.endtime=time(nullptr);
    a.CustomerStatus="已完成";
    stk.push(a);
    cout<<"叫号成功"<<endl;
    cout<<"姓名："<<a.name<<endl;
    cout<<"ID:"<<a.customerID<<endl;
    cout<<"序号："<<a.queueNumber<<endl;
    cout<<"入队时间："<<timeToString(a.arrivetime)<<endl;
    cout<<"叫号(出队)时间:"<<timeToString(a.endtime)<<endl;
    cout<<"等待时间(s):"<<endl;
    cout<<a.endtime-a.arrivetime;
}

void QueueSystem::DispQueue(){
    Customer a;
    cout<<"当前排队人数："<<length<<endl;
    time_t currentTime=time(nullptr);
    for(int i=0;i<length;i++){
        Getfront(a);
        cout<<"姓名："<<a.name<<" ";
        cout<<"ID:"<<a.customerID<<" ";
        cout<<"序号："<<a.queueNumber<<" ";
        cout<<a.CustomerStatus<<" ";
        cout<<"等待时间(s):"<<currentTime-a.arrivetime<<endl;
        cout<<endl;
        pop();
        push(a);
    }
}

void QueueSystem::CancelCall(){
    if(stk.empty()){
        cout<<"无已完成订单，不支持撤销叫号";
        return ;
    }
    Customer a;
    a=stk.top();
    stk.pop();
    a.endtime=0;
    a.CustomerStatus="排队中";
    push(a);
    cout<<"撤销叫号成功";
    cout<<"顾客"<<a.name<<"已重新入队";
}

void QueueSystem::ClearQueue(){
    cout<<"即将清空队列"<<endl;
    cout<<"输入 yes 确认"<<endl;
    string str1;
    cin>>str1;
    if(str1=="yes"){
        while(!empty()){
            pop();
        }
        cout<<"队列已清空"<<endl;
    }else return;

}

