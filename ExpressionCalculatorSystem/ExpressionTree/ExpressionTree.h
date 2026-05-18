#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H
#include"ExpressionTreeNode.h"
#include<string>
using namespace std;
class ExpressionTree{
    ExpressionTreeNode*r;
    void clear(ExpressionTreeNode* t);
    double getValue(ExpressionTreeNode* t);
    void DispTree(ExpressionTreeNode* t,int level);
    bool isOperator(const string& str);
    double operate(double a,string op,double b);
    public:
    ExpressionTree():r(nullptr){};
    ~ExpressionTree();
    void ClearTree();
    bool BuildFromPost(string postexp);//由后缀表达式建立表达式树
    bool BuildFromInfix(string infixexp);//由中缀表达式建立表达式树
    double GetValue();//计算表达式树的值
    void DispTree();//显示表达式树
};

#endif
