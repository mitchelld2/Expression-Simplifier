# lab5-expression-simplifier-f21

In this assignment you will read in an algebraic expression written in postfix and turn it into a binary expression tree.  The algebraic expressions can contain numbers and variables, but the operations are limited to + (addition), - (subtraction), and *(multiplication).  After building the expression tree, you will perform simplifications on the expression tree.  
The following class, declared in _ExpressionTree.h_, will represent an expression tree:
```
class ExpressionTree {
public:
    ExpressionTree();
    ~ExpressionTree();

    bool BuildExpressionTree(const string& postfix);
    void Simplify() { _root = SimplifyTree(_root); };

    friend ostream& operator<<(ostream& os, const ExpressionTree& tree) {
        return os << ToString(tree._root, false);
    }

private:
    static TreeNode* SimplifyTree(TreeNode* tree);
    static string ToString(TreeNode* tree, bool NeedOuterParen);
    static bool IsSameTree(TreeNode* tree1, TreeNode* tree2);

    TreeNode* _root;
};
```

The core of this assignment is to implement the  _BuildExpressionTree_ and _SimplifyTree_ methods in the file _ExpressionTree.cpp_.  You also will need to implement the default constructor and destructor, and you may find it useful to implement _IsSameTree_ to help you perform a couple of the simplifications.

To make sure you get off to a good start the implementation of the _ToString_ method has been supplied. 

The following class, declared in _TreeNode.h_, defines the tree nodes that will be used to build up an expression tree:

```
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
    bool IsOne() const { return _nodeType == NumberOperand && _data == "0";};
    bool SplitNumTimesVariable(int& c, TreeNode** tree) const;

private:
    NodeType _nodeType;
    string _data;
    TreeNode* _left;
    TreeNode* _right;
};
```

Many of these methods are implemented inline for you, but you will be required to implement its constructor and destructor in _TreeNode.h_.  You may find it useful to implement _SplitNumTimesVariable_ when you implement the final type of simplification.

The implementation of a simple templated _Stack_ class is supplied for you in the file _Stack.h_.  It is implemented by composition with an underlying _VariableArrayList_ class.

## Step 1:  Implement the constructors and destructors for _ExpressionTree_ and _TreeNode_

None of these are complicated, but it is important for you have a good understanding of the management of allocated memory.   The constructor for _ExpressionTree_ should simply set the _ _root_ to nullptr.  _BuildExpressionTree_  is in charge of building up the tree structure with its linkage of _TreeNode_ objects.

## Step 2:  Implement _BuildExpressionTree_

In class we’ve talked about how you can build a postfix calculator by using a stack.  What _BuildExpressionTree_ is very similar, except instead of calculating a number, it builds up an expression tree as it goes.  Here’s an outline of how it works:

```
1.	Initialize a streamstream object with the postfix string
2.	Initialize a stack of pointers to TreeNode objects
3.	Loop reading a token from the stringstream on each iteration
    a.	If the token is a number or a variable
        i.	Create a TreeNode object with appropriate NodeType and with the token as the data
        ii.	Push the TreeNode object onto the stack
    b.	Else if the token is an operator
        i.	If the stack has less than 2 items on it, print an error, clear the stack and return false
        ii.	Pop operand2 from the stack
	iii.	Pop operand1 from the stack
	iv.	Create a TreeNode object with appropriate NodeType and with the token as the data
            (1) Make operand1 its left subtree
            (2) Make operand2 its right subtree
        v.	Push your TreeNode object
    c.	Else print an error, clear the stack, and return false
4.	At this point the stack should contain 1 item.  If not, print an error, clear the stack and return false
5.	Pop the item from the stack.  This is the root of your expression tree.
6.	Return true
```

Notes:
1.  Using the supplied functions _IsNumber_, _IsVariable_, and _IsOperator_ to determine the type of your token may save you some work.
2.  Since your stack contains _pointers_ to allocated objects, if you hit an error, you will need to pop all items on the stack and _delete_ each of them.

## Step 3: Make sure what you have now works correctly

At this point make sure you can type in a line of postfix and have it output the the corresponding infix. You need to leave a blank between each number, variable, and operator.  That’s how the _stringstream_ object breaks it into tokens.

![image](https://user-images.githubusercontent.com/58283777/139603132-d69cfa73-a3b3-4625-a617-2f6ee6f8baaf.png)

Right now, the _SimplifyTree_ method just returns what is given, so the infix for “Infix:” and “Simplified:” will be exactly the same.  The main program operates in a loop so you can enter multiple lines.  When you are done, type Control-D to signal end of file and the main program will terminate when you are running your program on the command line.
 
You should also try the supplied input `postfix.txt`.  Copy this into your `cmake-build-debug` directory and run as shown:

![image](https://user-images.githubusercontent.com/58283777/139603216-7537ba2e-4182-4a39-a615-6e16232f8a6b.png)

This file contains a number of lines of postfix expressions.  They are grouped in categories.  Each group starts with a comment which the main program recognizes and simply prints out.

Finally, use _valgrind_ to make sure you are handling memory correctly.

## Step 4: Implement the _SimplifyTree_ method

Simplification should be done recursively.  You should call _SimplifyTree_ on the left subtree and on the right subtree before performing other simplifications.

The rest of your work can be done incrementally.  It is strongly recommended that once you finish one simplification, you make sure it runs property and _valgrind_ reports no problems before implementing the next simplification.

Here are the simplifications you should try to do:

1.	Simple arithmetic
    a.	If the tree is the addition of two number operands, return a new tree which is a leaf node storing the sum.
    b.	If the tree is the subtraction of two number operands, return a new tree which is a leaf node storing the difference.
    c.	If the tree is the multiplication of two number operands, return a new tree which is a leaf node storing the product.
2.	Multiplication by 0
    a.	If the tree is multiplication, and 0 is on the left or the right, then replace the tree with a leaf node storing 0.
3.	Adding or subtracting zero
    a.	If the tree is addition, and 0 is on the left, then replace the tree with the right subtree
    b.	If the tree is addition, and 0 is on the right, then replace the tree with the left subtree
    c.	If the tree is subtraction, and 0 is on the right, then replace the tree with the left subtree
4.	Multiplication by 1
    a.	If the tree is multiplication, and 1 is on the left, then replace the tree with the right subtree
    b.	If the tree is multiplication, and 1 is on the right, then replace the tree with the left subtree
5.	Subtraction of equal quantities
    a.	If the tree is subtraction, and the left and the right are the same, then replace the tree with a leaf node storing 0
6.	Change to customary order
    a.	If the tree is multiplication and right is a number operand, then swap the left and right trees  
7.	Distributive Law
    a.	If the tree is addition, and left subtree (L) is multiplication with a number on the left, and the right subtree (R) is also a multiplication with a number on the left, and the right subtrees of L and R are the same, then apply the distributive law
    b.	If the tree is subtraction, and left subtree (L) is multiplication with a number on the left, and the right subtree (R) is also a multiplication with a number on the left, and the right subtrees of L and R are the same, then apply the distributive law

Notes:
1.	Using the _TreeNode_ methods _IsNumber_, _IsOne_, and _IsZero_ may save you some time and simplify your code.
2.	The _stoi_ function may be used to convert the _string_ representation of a number to an integer
3.	The _to_string_ function may be used to convert an integer to a _string_
4.	For simplification step 5, you may want to implement the _IsSameTree_ method of _ExpressionTree_ and use that in your code.
5.	For simplification step 7, in addition to the _IsSameTree_ method, you may also find that implementing the _SplitNumTimesVariable_ method of _TreeNode_ to be helpful.

As you finish the simplification steps, make sure that you can simplify the corresponding lines of postfix in `postfix.txt`.
