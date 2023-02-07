//
// Interface Definition for the ExpressionTree Class
// Author: Max Benson
// Date: 10/27/2021
//
#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H

#include "TreeNode.h"

class ExpressionTree {
public:
    ExpressionTree();
    ~ExpressionTree();

    bool BuildExpressionTree(const string& postfix);
    void Simplify() { _root = SimplifyTree(_root); };

    friend ostream& operator<<(ostream& os, const ExpressionTree& tree) {
        return os << tree.ToString(tree._root, false);
    }

private:
    TreeNode* SimplifyTree(TreeNode* tree);
    string ToString(TreeNode* tree, bool NeedOuterParen) const;
    bool IsSameTree(TreeNode* tree1, TreeNode* tree2) const;

    TreeNode* _root;
};

#endif //EXPRESSIONTREE_H
