#include"ExpressionCalculator.h"
#include "../../Main/Function/Function.h"
#include<iostream>
#include<string>
#include<fstream>
#include<stack>
#include<vector>
#include<sstream>
#include<iomanip>
using namespace std;

ExpressionCalculator::ExpressionCalculator(){
    infixExpr="";
    postExpr="";
    preExpr="";
    result=0;
    hasResult=false;
    historyFileName=createHistoryFileName();
}

string ExpressionCalculator::createHistoryFileName(){
    time_t now=time(nullptr);
    tm* localTime=localtime(&now);
    stringstream ss;
    ss<<put_time(localTime,"ExpressionCalculatorSystem/HistoryExpression/history_%Y-%m-%d_%H-%M-%S.txt");
    return ss.str();
}

bool ExpressionCalculator::SetExpression(const string&str){
    string oldExpr=infixExpr;
    string oldPostExpr=postExpr;
    string oldPreExpr=preExpr;
    double oldResult=result;
    bool oldHasResult=hasResult;
    infixExpr=str;

    if(!IsValidExpression()){
        infixExpr=oldExpr;
        postExpr=oldPostExpr;
        preExpr=oldPreExpr;
        result=oldResult;
        hasResult=oldHasResult;
        return false;
    }

    postExpr="";
    preExpr="";
    result=0;
    hasResult=false;
    tree.ClearTree();

    if(!AutoMakeExpression()){
        infixExpr=oldExpr;
        postExpr=oldPostExpr;
        preExpr=oldPreExpr;
        result=oldResult;
        hasResult=oldHasResult;
        return false;
    }

    return true;
}

void ExpressionCalculator::InputExpression(){
    cout<<"1,键盘输入"<<endl;
    cout<<"2,文件载入"<<endl;
    cout<<"请输入选择：";
    int choice=getMenuChoice();
    switch (choice)
    {
    case 1:
        InputFromKeyboard();
        break;
    case 2:
        InputFromFile();
        break;
    default:
        cout<<"输入错误，返回菜单。"<<endl;
        return;
    }
    if(infixExpr!=""){
        cout<<"表达式为："<<infixExpr<<endl;
    }
}

void ExpressionCalculator::InputFromKeyboard(){
    string newExpr;
    cout<<"请输入中缀表达式，不要输入空格和负数：";
    cin>>newExpr;
    if(SetExpression(newExpr)){
        cout<<"表达式输入成功"<<endl;
    }
}

void ExpressionCalculator::InputFromFile(){
    string filename;
    string newExpr;
    cout<<"请输入文件名：";
    cin>>filename;
    ifstream fin(filename);
    if(!fin.is_open()){
        cout<<"文件打开失败";
        cout<<"检查文件是否存在或文件路径是否正确";
        return;
    }
    getline(fin,newExpr);
    if(newExpr==""){
        cout<<"读取失败";
        return;
    }
    if(SetExpression(newExpr)){
        cout<<"表达式读取成功"<<endl;
    }
}

bool ExpressionCalculator::AutoMakeExpression(){
    stack<char>optr;
    postExpr="";
    preExpr="";

    for(int i=0;i<infixExpr.length();i++){
        char ch=infixExpr[i];
        if(isNumberChar(ch)){
            while(i<infixExpr.length()&&isNumberChar(infixExpr[i])){
                postExpr+=infixExpr[i];
                i++;
            }
            postExpr+=" ";
            i--;
        }else if(ch=='('){
            optr.push(ch);
        }else if(ch==')'){
            while(!optr.empty()&&optr.top()!='('){
                postExpr+=optr.top();
                postExpr+=" ";
                optr.pop();
            }
            if(!optr.empty())optr.pop();
        }else if(isOperatorChar(ch)){
            while(!optr.empty()&&optr.top()!='('&&getPriority(optr.top())>=getPriority(ch)){
                postExpr+=optr.top();
                postExpr+=" ";
                optr.pop();
            }
            optr.push(ch);
        }
    }

    while(!optr.empty()){
        postExpr+=optr.top();
        postExpr+=" ";
        optr.pop();
    }

    vector<string>pre;
    for(int i=infixExpr.length()-1;i>=0;i--){
        char ch=infixExpr[i];
        if(isNumberChar(ch)){
            string num="";
            while(i>=0&&isNumberChar(infixExpr[i])){
                num=infixExpr[i]+num;
                i--;
            }
            pre.push_back(num);
            i++;
        }else if(ch==')'){
            optr.push(ch);
        }else if(ch=='('){
            while(!optr.empty()&&optr.top()!=')'){
                string op="";
                op+=optr.top();
                pre.push_back(op);
                optr.pop();
            }
            if(!optr.empty())optr.pop();
        }else if(isOperatorChar(ch)){
            while(!optr.empty()&&optr.top()!=')'&&getPriority(optr.top())>getPriority(ch)){
                string op="";
                op+=optr.top();
                pre.push_back(op);
                optr.pop();
            }
            optr.push(ch);
        }
    }

    while(!optr.empty()){
        string op="";
        op+=optr.top();
        pre.push_back(op);
        optr.pop();
    }

    for(int i=pre.size()-1;i>=0;i--){
        preExpr+=pre[i];
        preExpr+=" ";
    }

    stack<double>opnd;
    stringstream ss(postExpr);
    string str;
    while(ss>>str){
        if(str=="+"||str=="-"||str=="*"||str=="/"){
            if(opnd.size()<2)return false;
            double b=opnd.top();
            opnd.pop();
            double a=opnd.top();
            opnd.pop();
            if(str=="/"&&b==0)return false;
            opnd.push(operate(a,str[0],b));
        }else{
            double value;
            stringstream ss1(str);
            ss1>>value;
            opnd.push(value);
        }
    }

    if(opnd.size()!=1)return false;
    result=opnd.top();
    hasResult=true;
    addHistory();
    return true;
}

bool ExpressionCalculator::IsValidExpression(){
    if(infixExpr==""){
        cout<<"表达式不能为空"<<endl;
        return false;
    }

    for(int i=0;i<infixExpr.length();i++){
        if(!isLegalChar(infixExpr[i])){
            cout<<"表达式中含有非法字符："<<infixExpr[i]<<endl;
            return false;
        }
    }

    if(!isBracketMatch(infixExpr)){
        cout<<"表达式括号不匹配"<<endl;
        return false;
    }

    if(!isNumberFormatValid(infixExpr)){
        cout<<"表达式数字格式错误"<<endl;
        return false;
    }

    if(!isOperatorPositionValid(infixExpr)){
        cout<<"表达式运算符或括号位置错误"<<endl;
        return false;
    }

    if(isDivideZero()){
        cout<<"除数不能为0"<<endl;
        return false;
    }

    return true;
}

//中缀表达式转换为后缀表达式
bool ExpressionCalculator::InfixToPost(){
    if(!IsValidExpression()){
        return false;
    }

    stack<char>optr;
    postExpr="";

    for(int i=0;i<infixExpr.length();i++){
        char ch=infixExpr[i];

        if(isNumberChar(ch)){
            while(i<infixExpr.length()&&isNumberChar(infixExpr[i])){
                postExpr+=infixExpr[i];
                i++;
            }
            postExpr+=" ";
            i--;
        }else if(ch=='('){
            optr.push(ch);
        }else if(ch==')'){
            while(!optr.empty()&&optr.top()!='('){
                postExpr+=optr.top();
                postExpr+=" ";
                optr.pop();
            }
            if(!optr.empty()){
                optr.pop();
            }
        }else if(isOperatorChar(ch)){
            while(!optr.empty()&&optr.top()!='('&&getPriority(optr.top())>=getPriority(ch)){
                postExpr+=optr.top();
                postExpr+=" ";
                optr.pop();
            }
            optr.push(ch);
        }
    }

    while(!optr.empty()){
        postExpr+=optr.top();
        postExpr+=" ";
        optr.pop();
    }

    cout<<"后缀表达式："<<postExpr<<endl;
    return true;
}

//中缀表达式转换为前缀表达式
bool ExpressionCalculator::InfixToPre(){
    if(!IsValidExpression()){
        return false;
    }

    stack<char>optr;
    vector<string>pre;
    preExpr="";

    for(int i=infixExpr.length()-1;i>=0;i--){
        char ch=infixExpr[i];

        if(isNumberChar(ch)){
            string num="";
            while(i>=0&&isNumberChar(infixExpr[i])){
                num=infixExpr[i]+num;
                i--;
            }
            pre.push_back(num);
            i++;
        }else if(ch==')'){
            optr.push(ch);
        }else if(ch=='('){
            while(!optr.empty()&&optr.top()!=')'){
                string op="";
                op+=optr.top();
                pre.push_back(op);
                optr.pop();
            }
            if(!optr.empty()){
                optr.pop();
            }
        }else if(isOperatorChar(ch)){
            while(!optr.empty()&&optr.top()!=')'&&getPriority(optr.top())>getPriority(ch)){
                string op="";
                op+=optr.top();
                pre.push_back(op);
                optr.pop();
            }
            optr.push(ch);
        }
    }

    while(!optr.empty()){
        string op="";
        op+=optr.top();
        pre.push_back(op);
        optr.pop();
    }

    for(int i=pre.size()-1;i>=0;i--){
        preExpr+=pre[i];
        preExpr+=" ";
    }

    cout<<"前缀表达式："<<preExpr<<endl;
    return true;
}

//根据后缀表达式求值
bool ExpressionCalculator::GetPostValue(){
    if(postExpr==""){
        if(!InfixToPost())return false;
    }

    stack<double>opnd;
    stringstream ss(postExpr);
    string str;

    while(ss>>str){
        if(str=="+"||str=="-"||str=="*"||str=="/"){
            if(opnd.size()<2){
                cout<<"后缀表达式有误"<<endl;
                return false;
            }
            double b=opnd.top();
            opnd.pop();
            double a=opnd.top();
            opnd.pop();
            if(str=="/"&&b==0){
                cout<<"除数不能为0"<<endl;
                return false;
            }
            opnd.push(operate(a,str[0],b));
        }else{
            double value;
            stringstream ss1(str);
            ss1>>value;
            opnd.push(value);
        }
    }

    if(opnd.size()!=1){
        cout<<"计算失败"<<endl;
        return false;
    }

    result=opnd.top();
    hasResult=true;
    cout<<"后缀表达式求值结果："<<result<<endl;
    addHistory();
    return true;
}

//根据中缀表达式直接求值，先转后缀再计算
bool ExpressionCalculator::GetInfixValue(){
    if(!InfixToPost())return false;

    stack<double>opnd;
    stringstream ss(postExpr);
    string str;

    while(ss>>str){
        if(str=="+"||str=="-"||str=="*"||str=="/"){
            if(opnd.size()<2){
                cout<<"表达式有误"<<endl;
                return false;
            }
            double b=opnd.top();
            opnd.pop();
            double a=opnd.top();
            opnd.pop();
            if(str=="/"&&b==0){
                cout<<"除数不能为0"<<endl;
                return false;
            }
            opnd.push(operate(a,str[0],b));
        }else{
            double value;
            stringstream ss1(str);
            ss1>>value;
            opnd.push(value);
        }
    }

    if(opnd.size()!=1){
        cout<<"计算失败"<<endl;
        return false;
    }

    result=opnd.top();
    hasResult=true;
    cout<<"中缀表达式求值结果："<<result<<endl;
    addHistory();
    return true;
}

//根据表达式树求值
bool ExpressionCalculator::GetTreeValue(){
    if(!IsValidExpression())return false;
    if(!tree.BuildFromInfix(infixExpr)){
        return false;
    }
    result=tree.GetValue();
    hasResult=true;
    cout<<"表达式树求值结果："<<result<<endl;
    addHistory();
    return true;
}

void ExpressionCalculator::DispExpressionTree(){
    if(postExpr==""){
        if(!InfixToPost())return;
    }
    //这里用后缀表达式建立表达式树
    if(tree.BuildFromPost(postExpr)){
        tree.DispTree();
    }
}

//输出当前表达式相关结果
void ExpressionCalculator::DispResult(){
    if(infixExpr==""){
        cout<<"当前没有表达式"<<endl;
        return;
    }
    if(postExpr=="")InfixToPost();
    if(preExpr=="")InfixToPre();
    if(!hasResult){
        GetPostValue();
    }

    cout<<"================ 表达式结果 ================"<<endl;
    cout<<"中缀表达式："<<infixExpr<<endl;
    cout<<"后缀表达式："<<postExpr<<endl;
    cout<<"前缀表达式："<<preExpr<<endl;
    if(hasResult){
        cout<<"表达式值："<<result<<endl;
    }else{
        cout<<"表达式值：尚未求值"<<endl;
    }
    cout<<"============================================"<<endl;
}

void ExpressionCalculator::SaveResultToFile(){
    ofstream fout(historyFileName,ios::out);
    if(!fout.is_open()){
        cout<<"文件打开失败，结果保存失败"<<endl;
        return;
    }

    fout<<"表达式计算历史"<<endl;
    if(historyRecords.empty()){
        fout<<"暂无计算历史"<<endl;
    }else{
        for(int i=0;i<historyRecords.size();i++){
            fout<<"["<<i+1<<"] "<<historyRecords[i]<<endl;
        }
    }
    fout.close();
    cout<<"表达式结果已经保存到文件："<<historyFileName<<endl;
}

void ExpressionCalculator::DispHistory(){
    if(historyRecords.empty()){
        cout<<"暂无计算历史"<<endl;
        return;
    }

    cout<<"--- 计算历史 ---"<<endl;
    for(int i=0;i<historyRecords.size();i++){
        cout<<"["<<i+1<<"] "<<historyRecords[i]<<endl;
    }
}

void ExpressionCalculator::ClearHistory(){
    historyRecords.clear();

    ofstream fout(historyFileName,ios::out);
    if(!fout.is_open()){
        cout<<"计算历史清空失败"<<endl;
        return;
    }

    fout<<"表达式计算历史"<<endl;
    fout.close();
    cout<<"计算历史已清空"<<endl;
}

void ExpressionCalculator::ClearExpression(){
    infixExpr="";
    postExpr="";
    preExpr="";
    result=0;
    hasResult=false;
    tree.ClearTree();
    cout<<"当前表达式已清空"<<endl;
}

//简单演示栈处理表达式的过程
void ExpressionCalculator::ShowAlgorithm(){
    cout<<"表达式计算器算法演示"<<endl;
    cout<<"1. 中缀转后缀: 操作数直接输出，运算符先进运算符栈"<<endl;
    cout<<"2. 遇到优先级低的运算符时，栈顶运算符出栈到后缀式"<<endl;
    cout<<"3. 后缀求值: 操作数入栈，运算符弹出两个操作数计算"<<endl;
    cout<<"例如: 2*(4+9/3)"<<endl;
    cout<<"后缀式: 2 4 9 3 / + *"<<endl;
    cout<<"计算结果: 14"<<endl;
}

//判断是否为表达式中允许出现的字符
bool ExpressionCalculator::isLegalChar(char ch) const{
    if(isNumberChar(ch)){
        return true;
    }
    if(isOperatorChar(ch)){
        return true;
    }
    if(ch=='('||ch==')'){
        return true;
    }
    return false;
}

//用栈判断括号是否匹配
bool ExpressionCalculator::isBracketMatch(const string& expr) const{
    stack<char>bracket;

    for(int i=0;i<expr.length();i++){
        if(expr[i]=='('){
            bracket.push(expr[i]);
        }else if(expr[i]==')'){
            if(bracket.empty()){
                return false;
            }
            bracket.pop();
        }
    }

    return bracket.empty();
}

//判断数字格式是否合法
bool ExpressionCalculator::isNumberFormatValid(const string& expr) const{
    for(int i=0;i<expr.length();i++){
        if((expr[i]>='0'&&expr[i]<='9')||expr[i]=='.'){
            int point_count=0;
            int digit_count=0;

            while(i<expr.length()&&((expr[i]>='0'&&expr[i]<='9')||expr[i]=='.')){
                if(expr[i]=='.'){
                    point_count++;
                }else{
                    digit_count++;
                }

                if(point_count>1){
                    return false;
                }
                i++;
            }

            if(digit_count==0){
                return false;
            }
            i--;
        }
    }

    return true;
}

//判断运算符和括号的位置是否合法
bool ExpressionCalculator::isOperatorPositionValid(const string& expr) const{
    char last='\0';
    bool has_last=false;

    for(int i=0;i<expr.length();i++){
        char ch=expr[i];

        if(isOperatorChar(ch)){
            if(!has_last||last=='('||isOperatorChar(last)){
                return false;
            }
        }else if(ch=='('){
            if(has_last&&(isNumberChar(last)||last==')')){
                return false;
            }
        }else if(ch==')'){
            if(!has_last||last=='('||isOperatorChar(last)){
                return false;
            }
        }else if(isNumberChar(ch)){
            if(has_last&&last==')'){
                return false;
            }
        }

        last=ch;
        has_last=true;
    }

    if(!has_last||isOperatorChar(last)||last=='('){
        return false;
    }

    return true;
}

//简单检查直接除0的情况
bool ExpressionCalculator::isDivideZero() const{
    for(int i=0;i<infixExpr.length();i++){
        if(infixExpr[i]=='/'){
            int j=i+1;
            if(j<infixExpr.length()&&infixExpr[j]=='('){
                j++;
            }

            string num="";
            while(j<infixExpr.length()&&isNumberChar(infixExpr[j])){
                num+=infixExpr[j];
                j++;
            }

            if(j<infixExpr.length()&&infixExpr[j]==')'){
                j++;
            }

            if(num!=""){
                stringstream ss(num);
                double value;
                ss>>value;
                if(value==0){
                    return true;
                }
            }
        }
    }
    return false;
}

bool ExpressionCalculator::isOperatorChar(char ch) const{
    return ch=='+'||ch=='-'||ch=='*'||ch=='/';
}

bool ExpressionCalculator::isNumberChar(char ch) const{
    return (ch>='0'&&ch<='9')||ch=='.';
}

int ExpressionCalculator::getPriority(char op) const{
    if(op=='+'||op=='-'){
        return 1;
    }
    if(op=='*'||op=='/'){
        return 2;
    }
    return 0;
}

double ExpressionCalculator::operate(double a,char op,double b){
    if(op=='+')return a+b;
    if(op=='-')return a-b;
    if(op=='*')return a*b;
    if(op=='/')return a/b;
    return 0;
}

void ExpressionCalculator::addHistory(){
    stringstream ss;
    ss<<infixExpr<<" = "<<result;
    string str=ss.str();
    if(!historyRecords.empty()&&historyRecords.back()==str){
        return;
    }
    historyRecords.push_back(str);
}

bool ExpressionCalculator::hasHistoryRecords(){
    return historyRecords.size()>0;
}
