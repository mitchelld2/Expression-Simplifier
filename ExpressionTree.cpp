//
// Implements the ExpressionTree Class
// Author: Max Benson
// Date: 10/27/2021
//

#include <iostream>
#include <sstream>
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::to_string;
using std::stoi;
using std::equal;

#include "Stack.h"
#include "ExpressionTree.h"

// Token testing routines
bool IsNumber(string token);
bool IsVariable(string token);
bool IsOperator(string token);

/**
 * Default constructor
 * Creates an "null tree"
 */
ExpressionTree::ExpressionTree()
{
    _root= nullptr;
}

/**
 * Destructor
 * Frees the dynamic memory allocated for the tree
 */
ExpressionTree::~ExpressionTree()
{
    delete _root;
}

/**
 * Build an expression tree from its postfix representation
 * In case of error the stack is cleaned up.  Because it contains
 * pointers to TreeNodes, if any are left on the stack they must be
 * explicitly deleted
 * @param postfix string representation of tree
 * @return true if postfix valid and tree was built, false otherwise
 */
bool ExpressionTree::BuildExpressionTree(const string& postfix) {
    stringstream ss(postfix);   //1.	Initialize a streamstream object with the postfix string
    string token;

    Stack<TreeNode*> branch;    //2.	Initialize a stack of pointers to TreeNode objects
    TreeNode* node;

    while(ss >> token)  //3.	Loop reading a token from the stringstream on each iteration
    {
        if(IsNumber(token))    //a.	If the token is a number or a variable
        {
            node = new TreeNode (NumberOperand, token); //        i.	Create a TreeNode object with appropriate NodeType and with the token as the data
            branch.Push(node);  //        ii.	Push the TreeNode object onto the stack
        }
        else if(IsVariable(token))
        {
            node = new TreeNode (VariableOperand, token);
            branch.Push(node);
        }
        else if(IsOperator(token))  //    b.	Else if the token is an operator
        {
            if(branch.Size() < 2)  //        i.	If the stack has less than 2 items on it, print an error, clear the stack and return false
            {
                cout << "Error";
                while(!branch.IsEmpty()){branch.Pop();}
                return false;
            }

            node = new TreeNode (Operator, token);
            node->SetRight(branch.Pop());            //ii.	Pop operand2 from the stack
            node->SetLeft(branch.Pop());   //iii.	Pop operand1 from the stack

            branch.Push(node);  //iv.	Create a TreeNode object with appropriate NodeType and with the token as the data
        }
        else    //    c.	Else print an error, clear the stack, and return false
        {
            cout << "Error";
            while(!branch.IsEmpty()){branch.Pop();}
            return false;
        }
    }

    if(branch.Size() != 1) //4.	At this point the stack should contain 1 item.  If not, print an error, clear the stack and return false
    {
        while(!branch.IsEmpty()){branch.Pop();}
        return false;
    }

    _root = branch.Pop();  //5.	Pop the item from the stack.  This is the root of your expression tree.
    return true;    //6.	Return true
}

/**
 * Recursively simplify an expression stored in an expression tree.  THe following simplications are performed
 * - Addition, multiplication, and subtraction of constants is performed reducing the subtree to a leaf containing a number
 * - 0 + exp, exp + 0, exp - 0  will be reduced to exp, in general exp will a tree
 * - 1 * exp, exp * 1  will be reduced to exp, in general exp will a tree
 * - 0 * exp, exp * 0  will be reduce to a leaf containing 0
 * - exp - exp will be reduce to a leaf containing 0
 * - exp * number will be changed to number * exp
 * - (c1 * exp) + (c2 * exp) where c1, c2 are numbers  will be changed to (c1+c2) * exp
 * - (c1 * exp) - (c2 * exp) where c1, c2 are numbers will be changed to (c1-c2) * exp
 * pointers to TreeNodes, if any are left on the stack they must be
 * explicitly deleted
 * @param postfix string representation of tree
 * @return true if postfix valid and tree was built, false otherwise
 */
TreeNode* ExpressionTree::SimplifyTree(TreeNode* tree)
{
    TreeNode* node;
    if(!tree->Left()->IsNumber() && !IsVariable(tree->Left()->Data()))
    {
        tree->SetLeft(SimplifyTree(tree->Left()));
    }
    if(!tree->Right()->IsNumber() && !IsVariable(tree->Right()->Data()))
    {
        tree->SetRight(SimplifyTree(tree->Right()));
    }

    if(IsNumber(tree->Left()->Data()) && IsNumber(tree->Right()->Data()))
    {
        int leftVal = stoi(tree->Left()->Data());
        int rightVal = stoi(tree->Right()->Data());
        int val = 0;

        if(tree->Data() == "+")
        {
            val = leftVal + rightVal;
        }
        else if(tree->Data() == "*")
        {
            val = leftVal * rightVal;
        }
        else if(tree->Data() == "-")
        {
            val = leftVal - rightVal;
        }

        node = new TreeNode (NumberOperand, to_string(val));
    }
    else if((tree->Left()->Type() == VariableOperand || tree->Right()->Type() == VariableOperand))
    {
        string leftVar, rightVar;
        leftVar += tree->Left()->Data();
        rightVar += tree->Right()->Data();
        string val;

        if(tree->Data() == "+")
        {
            if(leftVar == "0")
            {
                val = rightVar;
            }
            else if(rightVar == "0")
            {
                val = leftVar;
            }
            else
            {
                val = "(" + leftVar + "+" + rightVar + ")";
            }
        }
        else if(tree->Data() == "*")
        {
            if(leftVar == "0" || rightVar == "0")
            {
                val = "0";
            }
            else if(leftVar == "1")
            {
                val = rightVar;
            }
            else if(rightVar == "1")
            {
                val = leftVar;
            }
            else
            {
                val = "(" + leftVar + "*" + rightVar + ")";
            }
        }
        else if(tree->Data() == "-")
        {
            if(leftVar == rightVar)
            {
                val = "0";
            }
            else if(leftVar == "0") //0
            {
                val = "(-" + rightVar + ")";
            }
            else if(rightVar == "0")
            {
                val = leftVar;
            }
            else
            {
                val = "(" + leftVar + '-' + rightVar + ")";
            }
        }

        node = new TreeNode (VariableOperand, val);
    }
    return node;
}

/**
 * Determine whether two tree structures represent the same expression
 * @param tree1 first tree structure
 * @param tree2 second tree structure
 * @return true if same, false otherwise
 */
bool ExpressionTree::IsSameTree(TreeNode* tree1, TreeNode* tree2) const
{
    return false;
}

/**
 * Produce an infix representation of the tree structure
 * @param tree
 * @param fNeedOuterParen - caller will generatlly pass false to eliminate outer set of paraentheses, recursive calls pass true
 * @return string representation
 */
string ExpressionTree::ToString(TreeNode* tree, bool fNeedOuterParen) const {
    string s;

    if (Operator == tree->Type()) {
        if (fNeedOuterParen) {
            s += "(";
        }
        s += ToString(tree->Left(), true);
        s += tree->Data();
        s += ToString(tree->Right(), true);
        if (fNeedOuterParen) {
            s += ")";
        }
    } else {
        s += tree->Data();
    }
    return s;
}

/**
 * Scans token to see if all characters are digits
 * @param token a string
 * @return true if nonempty and contains all digits, otherwise false
 */
bool IsNumber(string token) {
    if (token.length() == 0) {
        return false;
    }
    for (size_t i = 0; i < token.length(); i ++) {
        if (!isdigit(token[i]) ) {
            return false;
        }
    }
    return true;
}

/**
 * Scans token to see if it starts with letter and rest are letter or digits
 * @param token a string
 * @return true if nonempty,starts with letter, and rest are letters or digits
 */
bool IsVariable(string token)  {
    if (token.length() == 0 || !isalpha(token[0])) {
        return false;
    }
    for (size_t i = 0; i < token.length(); i ++) {
        if (!isalnum(token[i]) ) {
            return false;
        }
    }
    return true;
}

/**
 * CHecks if the stirng is a +, -, or *
 * @param token a string
 * @return true if its an operator
 */
bool IsOperator(string token)  {
    return (token.length() == 1 && (token[0] == '+' || token[0] == '-' || token[0] == '*'));
}
