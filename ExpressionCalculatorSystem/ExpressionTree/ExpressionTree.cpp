#include "ExpressionTree.h"
#include <iostream>
#include <sstream>
#include <stack>
using namespace std;

ExpressionTree::~ExpressionTree(){
    ClearTree();
}

void ExpressionTree::clear(ExpressionTreeNode* t){
    if(t==nullptr)return;
    clear(t->lchild);
    clear(t->rchild);
    delete t;
}

void ExpressionTree::ClearTree(){
    clear(r);
    r=nullptr;
}

bool ExpressionTree::isOperator(const string& str){
    return str=="+"||str=="-"||str=="*"||str=="/";
}

double ExpressionTree::operate(double a,string op,double b){
    if(op=="+")return a+b;
    if(op=="-")return a-b;
    if(op=="*")return a*b;
    if(op=="/"){
        if(b==0){
            cout<<"除数不能为0"<<endl;
            return 0;
        }
        return a/b;
    }
    return 0;
}

//由后缀表达式建树，遇到数字入栈，遇到运算符弹出两个结点
bool ExpressionTree::BuildFromPost(string postexp){
    ClearTree();
    stack<ExpressionTreeNode*>stk;
    stringstream ss(postexp);
    string str;

    while(ss>>str){
        ExpressionTreeNode* node=new ExpressionTreeNode(str);
        if(!isOperator(str)){
            stk.push(node);
        }else{
            if(stk.size()<2){
                delete node;
                cout<<"表达式树建立失败";
                return false;
            }
            node->rchild=stk.top();
            stk.pop();
            node->lchild=stk.top();
            stk.pop();
            stk.push(node);
        }
    }

    if(stk.size()!=1){
        cout<<"表达式树建立失败";
        while(!stk.empty()){
            delete stk.top();
            stk.pop();
        }
        return false;
    }

    r=stk.top();
    return true;
}

//由中缀表达式建树，先转成后缀表达式
bool ExpressionTree::BuildFromInfix(string infixexp){
    stack<char>optr;
    string postexp="";

    for(int i=0;i<infixexp.length();i++){
        char ch=infixexp[i];

        if((ch>='0'&&ch<='9')||ch=='.'){
            while(i<infixexp.length()&&((infixexp[i]>='0'&&infixexp[i]<='9')||infixexp[i]=='.')){
                postexp+=infixexp[i];
                i++;
            }
            postexp+=" ";
            i--;
        }else if(ch=='('){
            optr.push(ch);
        }else if(ch==')'){
            while(!optr.empty()&&optr.top()!='('){
                postexp+=optr.top();
                postexp+=" ";
                optr.pop();
            }
            if(!optr.empty()){
                optr.pop();
            }
        }else if(ch=='+'||ch=='-'||ch=='*'||ch=='/'){
            int chPriority=(ch=='+'||ch=='-')?1:2;
            while(!optr.empty()&&optr.top()!='('){
                int topPriority=(optr.top()=='+'||optr.top()=='-')?1:2;
                if(topPriority<chPriority)break;
                postexp+=optr.top();
                postexp+=" ";
                optr.pop();
            }
            optr.push(ch);
        }
    }

    while(!optr.empty()){
        postexp+=optr.top();
        postexp+=" ";
        optr.pop();
    }

    return BuildFromPost(postexp);
}

double ExpressionTree::getValue(ExpressionTreeNode* t){
    if(t==nullptr)return 0;
    if(!isOperator(t->data)){
        stringstream ss(t->data);
        double value;
        ss>>value;
        return value;
    }
    double a=getValue(t->lchild);
    double b=getValue(t->rchild);
    return operate(a,t->data,b);
}

double ExpressionTree::GetValue(){
    return getValue(r);
}

void ExpressionTree::DispTree(ExpressionTreeNode* t,int level){
    if(t==nullptr)return;
    DispTree(t->rchild,level+1);
    for(int i=0;i<level;i++){
        cout<<"    ";
    }
    cout<<t->data<<endl;
    DispTree(t->lchild,level+1);
}

void ExpressionTree::DispTree(){
    if(r==nullptr){
        cout<<"表达式树为空"<<endl;
        return;
    }
    cout<<"表达式树如下："<<endl;
    DispTree(r,0);
}
