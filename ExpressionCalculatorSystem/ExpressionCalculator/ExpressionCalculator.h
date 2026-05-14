#ifndef EXPRESSIONCALCULATOR_H
#define EXPRESSIONCALCULATOR_H
#include"../ExpressionTree/ExpressionTree.h"
#include<string>
using namespace std;
class ExpressionCalculator{
    private:
    ExpressionTree tree;

    string infixExpr;//中缀表达式
    string postExpr;//后缀表达式
    string preExpr;//前缀表达式

    double result;
    ExpressionCalculator(){
        infixExpr="";
        postExpr="";
        preExpr="";
        result=0;
    }
    ~ExpressionCalculator(){}
    void inputExpression();
    void inputFromKeyboard();
    void inputFronFile();
};

#endif