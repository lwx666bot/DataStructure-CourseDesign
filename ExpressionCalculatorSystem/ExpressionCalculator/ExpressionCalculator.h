#ifndef EXPRESSIONCALCULATOR_H
#define EXPRESSIONCALCULATOR_H
#include "../ExpressionTree/ExpressionTree.h"
#include <string>
#include <vector>
using namespace std;
class ExpressionCalculator{
    private:
    ExpressionTree tree;

    string infixExpr;//中缀表达式
    string postExpr;//后缀表达式
    string preExpr;//前缀表达式

    double result;
    bool hasResult;//判断是否已经求值
    vector<string>historyRecords;//保存计算历史记录
    string historyFileName;//计算历史文件名称
    string createHistoryFileName();//生成本次运行程序的历史信息记录文件名称
    public:
    ExpressionCalculator();
    ~ExpressionCalculator(){}
    bool SetExpression(const string&str);
    void InputExpression();
    void InputFromKeyboard();
    void InputFromFile();
    bool IsValidExpression();
    bool InfixToPost();
    bool InfixToPre();
    bool GetInfixValue();
    bool GetPostValue();
    bool GetTreeValue();
    void DispExpressionTree();
    void DispResult();
    void SaveResultToFile();
    void DispHistory();
    void ClearHistory();
    void ClearExpression();
    void ShowAlgorithm();
    bool hasHistoryRecords();//判断是否有表达式计算历史
    private:
    //表达式验证的辅助函数
    bool isLegalChar(char ch) const;
    bool isBracketMatch(const string& expr) const;
    bool isNumberFormatValid(const string& expr) const;
    bool isOperatorPositionValid(const string& expr) const;
    bool isDivideZero() const;
    bool isOperatorChar(char ch) const;
    bool isNumberChar(char ch) const;
    int getPriority(char op) const;
    double operate(double a,char op,double b);
    void addHistory();
    bool autoMakeExpression();//输入后自动计算，不输出过程
};

#endif
