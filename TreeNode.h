//
// Interface Definition for the TreeNode Class
// Author: Max Benson
// Date: 10/27/2021
//
#ifndef TREENODE_H
#define TREENODE_H

#include <iostream>
using std::ostream;
using std::string;
using std::to_string;

enum NodeType {
    Operator,
    NumberOperand,
    VariableOperand
};

class TreeNode {
public:
    TreeNode(NodeType nodeType, string data);
    ~TreeNode();

    NodeType Type() const { return _nodeType; };
    string Data() const {return _data;};
    TreeNode *Left() const {return _left;};
    TreeNode *Right() const {return _right;};

    void SetLeft(TreeNode* left) {_left = left;};
    void SetRight(TreeNode* right) {_right = right;};

    bool IsNumber() const { return _nodeType == NumberOperand; };
    bool IsZero() const { return _nodeType == NumberOperand && _data == "0"; };
    bool IsOne() const { return _nodeType == NumberOperand && _data == "1";};
    bool SplitNumTimesVariable(int& c, TreeNode** tree) const;

private:
    NodeType _nodeType;
    string _data;
    TreeNode* _left;
    TreeNode* _right;
};

#endif //TREENODE_H
