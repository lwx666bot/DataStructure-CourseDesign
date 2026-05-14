#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "QueueSystem.h"
using namespace std;
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
//判断队列是否为空
bool QueueSystem::empty(){
    return front==rear;
}
//返回队列长度
int QueueSystem::Getlength(){
    return length;
}
//入队
bool QueueSystem::push(Customer& e){
    if((rear+1)%MaxSize==front){
        cout<<"队列已满";
        return false;
    }
    rear=(rear+1)%MaxSize;
    CustomerQueue[rear]=e;
    length++;
    return true;
}
//出队
bool QueueSystem::pop(){
    if(empty()){
        cout<<"队空";
        return false;
    }
    front=(front+1)%MaxSize;
    length--;
    return true;
}

void QueueSystem:: addCustomer(){
    string str;
    cout<<"请输入顾客姓名(输入-1取消本次排队)"<<endl;
    cin>>str;
    if(str=="-1")return;
    Customer a(AutoCustomerID,AutoQueueNumber,str); 
    bool flag=push(a);
    if(flag){
        AutoCustomerID++;
        AutoQueueNumber++;
        historyRecords.push_back(a);
        cout<<"入队成功"<<endl;
        cout<<"姓名："<<a.name<<endl;
        cout<<"ID:"<<a.customerID<<endl;
        cout<<"序号："<<a.queueNumber<<endl;
        cout<<"入队时间："<<timeToString(a.arrivetime);//
    }
    else cout<<"入队失败";
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
    int search=a.queueNumber-1;
    if(search<0||search>=historyRecords.size()){
        cout<<"历史记录中未找到该顾客";
        cout<<"叫号失败";
        return;
    }
    pop();
    historyRecords[search].endtime=a.endtime=time(nullptr);
    historyRecords[search].CustomerStatus= a.CustomerStatus="已完成";

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
        cout<<"状态:"<<a.CustomerStatus<<" ";
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
    int search=a.queueNumber-1;
    if(search<0||search>=(int)historyRecords.size()){
         cout<<"历史记录中未找到该顾客";
        cout<<"撤销叫号失败";
        return;
    }
    a.endtime=0;
    a.CustomerStatus="排队中";
    if(push(a)){
        historyRecords[search].endtime=0;
        historyRecords[search].CustomerStatus="排队中";
        stk.pop();
        cout<<"撤销叫号成功";
        cout<<"顾客"<<a.name<<"已重新入队";
    }else{
        cout<<"当前队列已满，无法撤销。";
    }
    
}

void QueueSystem::ClearQueue(){
    cout<<"即将清空队列"<<endl;
    cout<<"输入 yes 确认"<<endl;
    string str1;
    cin>>str1;
    if(str1=="yes"){
        while(!empty()){
            Customer a;
            Getfront(a);
            int search =a.queueNumber-1;
            if(search>=0&&search<historyRecords.size()){
                historyRecords[search].CustomerStatus="已取消";
                historyRecords[search].endtime=time(nullptr);
            }
            pop();
        }
        cout<<"队列已清空"<<endl;
    }else return;

}

void QueueSystem::DispHistory(){
    if(historyRecords.empty()){
        cout<<"暂无历史服务记录"<<endl;
        return;
    }

    cout<<"================ 历史服务记录 ================"<<endl;

    for(int i = 0; i < historyRecords.size(); i++){
        Customer c = historyRecords[i];

        cout<<"姓名："<<c.name<<endl;
        cout<<"ID："<<c.customerID<<endl;
        cout<<"排队号码："<<c.queueNumber<<endl;
        cout<<"入队时间："<<timeToString(c.arrivetime)<<endl;
        cout<<"叫号时间："<<timeToString(c.endtime)<<endl;
        cout<<"当前状态："<<c.CustomerStatus<<endl;

        if(c.CustomerStatus == "已完成"){
            cout<<"等待时间(s)："<<c.endtime - c.arrivetime<<endl;
        }
        else if(c.CustomerStatus=="已取消"){
            cout<<"等待时间(s):"<<c.endtime-c.arrivetime<<"  队列清空导致取消"<<endl;
        }
        else {
            cout<<"等待时间(s)：尚未完成叫号"<<endl;
        }
        cout<<"---------------------------------------------"<<endl;
    }
}

void QueueSystem::showSystem(){
    int canceledCount = 0;

    for(int i = 0; i < historyRecords.size(); i++){
        if(historyRecords[i].CustomerStatus == "已取消"){
            canceledCount++;
        }
    }
    double capacityUsageRate = length * 100.0 / (MaxSize - 1);
    double queueCongestionIndex = length * 0.6 + capacityUsageRate * 0.4;
    //同时参考当前等待人数和容量使用率衡量拥堵程度
    cout << "========== 系统当前状态 ==========" << endl;
    cout << "本次总取号人数：" << historyRecords.size() << endl;
    cout << "当前等待人数：" << length << endl;
    cout << "服务已完成人数：" << stk.size() << endl;
    cout << "清空队列取消人数：" << canceledCount << endl;
    cout << "----------------------------------" << endl;
    cout << fixed << setprecision(2);
    cout << "队列容量使用率：" << capacityUsageRate << "%" << endl;
    cout << "排队拥堵指数：" << queueCongestionIndex << endl;
    cout << "==================================" << endl;
}

string QueueSystem::createHistoryFileName(){
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    stringstream ss;
    ss << put_time(localTime, "RestaurantQueueSystem/HistoryRecords/history_%Y-%m-%d_%H-%M-%S.csv");
    //这里也可以修改后缀为csv，输出更像表格的csv文件
    return ss.str();
}

void QueueSystem::SaveHistoryToFile()
{
    ofstream fout(historyFileName, ios::out | ios::binary);

    if(!fout.is_open())
    {
        cout << "历史信息保存失败，文件无法打开！" << endl;
        cout << "请检查项目目录下是否已经创建 HistoryRecords 文件夹。" << endl;
        return;
    }
    fout << "\xEF\xBB\xBF";
    fout << "顾客ID,姓名,排队号码,入队时间,叫号时间,当前状态,等待时间(s)" << endl;
    double AllTime=0;
    for(int i = 0; i < historyRecords.size(); i++)
    {
        Customer c = historyRecords[i];
        if(c.CustomerStatus=="已完成"){
            AllTime=AllTime+(c.endtime-c.arrivetime);
        }
        fout << c.customerID << ",";//前面如果保存成csv文件，这里“，”可以分隔不同列
        fout << c.name << ",";
        fout << c.queueNumber << ",";
        fout << timeToString(c.arrivetime) << ",";
        fout << timeToString(c.endtime) << ",";
        fout << c.CustomerStatus << ",";

        if(c.CustomerStatus == "已完成" || c.CustomerStatus == "已取消")
        {
            fout << c.endtime - c.arrivetime;
        }
        else
        {
            fout << "尚未完成";
        }

        fout << endl;
    }
    if(stk.size()>0){
        double averageTime=AllTime/stk.size();
        fout<<"平均等待时间:"<<","<<averageTime;
    }else{
        fout<<"平均等待时间：无";
    }
    fout.close();

    cout << "历史信息已保存到文件：" << historyFileName << endl;
}

bool QueueSystem::hasHistoryRecords(){
    return historyRecords.size()>0;
}
