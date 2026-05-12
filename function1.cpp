#include <iostream>
#include<cstdlib>
using namespace std;
void print_No1(){
    system("cls");//清空终端
    cout<<"=====数据结构课程设计系统====="<<endl;
          cout<<"1,校园餐厅排队叫号模拟系统" <<endl;
         cout<< "2,表达式计算器"<<endl;
          cout<<"0,退出系统"<<endl;
          cout<<"请输出选项"<<endl;
}
void mainMenu(){
     int choice;
    while(true){
        print_No1();
        cin>>choice;
        switch(choice){
            case 0:{
                cout<<"退出系统...";
                break;
            }
            case 1:{
                //进行排队模拟相关操作
                break;
            }
            case 2:{
                //进行表达式计算器相关操作
                break;
            }
            default:{
                cout<<"无效输入，重新选择。。。";
            }
            
        }
    }
}
