#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <ctime>

struct Customer
{
    int customerID;//顾客ID，标识身份
    int queueNumber;//排队号码
    std::string name;//顾客名称
    time_t arrivetime;//记录入队时间
    time_t endtime;//记录叫号（出队）时间
    std::string CustomerStatus;//；记录顾客状态，如“排队中”“已完成”等，并且凭此统计信息
    Customer(int ID,int number,std::string name){
        customerID=ID;
        queueNumber=number;
        this->name=name;
        arrivetime=time(nullptr);
        endtime=0;
        CustomerStatus="排队中";
    }
    Customer()
    {
        customerID = 0;
        queueNumber = 0;
        name = "";
        arrivetime = 0;
        endtime=0;
        CustomerStatus="未初始化";
    }

    
};


#endif
