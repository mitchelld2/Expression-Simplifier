//
// Implements the TreeNode Class
// Author: Max Benson
// Date: 10/27/2021
//

#include <assert.h>
#include "TreeNode.h"

/**
 * Constructor
 * @param nodeType one of Operator, NumberOperand, or VariableOperand
 * @param data an operator (+, -, *), a number, or a variable name
 */
TreeNode::TreeNode(NodeType nodeType, string data)
{
    _nodeType = nodeType;
    _data = data;
}

/**
 * Destructor
 * Frees allocated memory
 */
TreeNode::~TreeNode()
{

}


/**
 * If it's a multiplcation node, and left is a number, return number on left, and expression tree on right
 * @param c receives number
 * @param ptree receives pointer to expression tree
 * @return true if node is a multiplication of number * exp, false otherwise
 */
bool TreeNode::SplitNumTimesVariable(int& c, TreeNode** ptree) const
{
    return false;
}
