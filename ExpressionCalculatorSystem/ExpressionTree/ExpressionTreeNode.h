#ifndef EXPRESSIONTREENODE_H
#define EXPRESSIONTREENODE_H
#include <string>
using namespace std;
struct ExpressionTreeNode{
    string data;
    ExpressionTreeNode *lchild;
    ExpressionTreeNode *rchild;
    ExpressionTreeNode():lchild(nullptr),rchild(nullptr){};
    ExpressionTreeNode(string data){
        this->data=data;
        lchild=nullptr;
        rchild=nullptr;
    }
};

#endif 