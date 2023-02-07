#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::getline;
#include "ExpressionTree.h"
int main() {
    string postfix;
    cout << "> ";
    while ( getline(cin, postfix) ) {
        if (postfix.length() == 0 || postfix[0] == '#') {
            cout << postfix << endl;
        }
        else {
            ExpressionTree expTree;
            cout << "Postfix: " << postfix << endl;
            if (expTree.BuildExpressionTree(postfix)) {
                cout << "Infix:  " << expTree << endl;
                expTree.Simplify();
                cout << "Simplified: " << expTree << endl;
            }
            cout << "> ";
        }
    }
    return 0;
}