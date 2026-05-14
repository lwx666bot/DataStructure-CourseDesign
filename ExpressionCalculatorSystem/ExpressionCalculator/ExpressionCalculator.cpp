#include"ExpressionCalculator.h"
#include "../../Main/Function/Function.h"
#include<iostream>
#include<string>
#include<fstream>
using namespace std;

bool ExpressionCalculator::setExpression(const string&str){
    infixExpr=str;
    return true;
}

void ExpressionCalculator::inputExpression(){
    cout<<"1,键盘输入";
    cout<<"2,文件载入";
    cout<<"请输入选择";
    int choice=getMenuChoice();
    switch (choice)
    {
    case 1:
        inputFromKeyboard();
        break;
    case 2:
        inputFromFile();
        break;
    default:
        cout<<"输入错误，返回菜单。";
        break;
    }
    cout<<"表达式为:"<<infixExpr;
}

void ExpressionCalculator::inputFromKeyboard(){
    string new_expr;
    cout<<"请输入中缀表达式。";
    cin>>new_expr;
    setExpression(new_expr);
}

void ExpressionCalculator::inputFromFile(){
    string filename;
    string new_expr;
    cout<<"请输入文件名";
    cin>>filename;
    ifstream fin(filename);
    if(!fin.is_open()){
        cout<<"文件打开失败";
        cout<<"检查文件是否存在或文件路径是否正确";
        return;
    }
    getline(fin,new_expr);
    if(new_expr==""){
        cout<<"读取失败";
        return;
    }
    setExpression(new_expr);
}








































