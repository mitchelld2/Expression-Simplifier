//
// Interface Definition for the template version of the Stack Class
// Author: Max Benson
// Date: 08/15/2021
//

#ifndef STACK_H
#define STACK_H

#include "VariableArrayList.h"

template <typename ValueType>
class Stack {
public:
    bool IsEmpty() const;
    size_t Size() const;

    bool Push(const ValueType& value);
    ValueType Peek();
    ValueType Pop();

    friend ostream& operator<<(ostream& os, const Stack& stack) {
        os << stack._list;
    }

private:
    VariableArrayList<ValueType> _list;
};

/**
* Returns whether stack is empty
* @return true if stack empty, false otherwise.
*/
template <typename ValueType>
bool Stack<ValueType>::IsEmpty() const {
    return _list.Size() == 0;
}

/**
* Returns number of value on stack
* @return number of values
*/
template <typename ValueType>
size_t Stack<ValueType>::Size() const {
    return _list.Size();
}

/**
* Puts the parameter "value" on top of the stack
* @param value
* @return true if successful, false otherwise
*/
template <typename ValueType>
bool Stack<ValueType>::Push(const ValueType& value) {
    return _list.Insert(_list.Size(), value);
}

/**
* Removes the top value on the stack and returns it
* Caller should make sure the stack is not empty.
* @return top value of stack
*/
template <typename ValueType>
ValueType Stack<ValueType>::Pop() {
    bool ret;
    ValueType value;

    ret = _list.Remove(_list.Size()-1,value);
    assert(ret);
    return value;
}

/**
* Returns value stored at the top of the stack
* Caller should make sure the stack is not empty.
* @return stack's top value.
*/
template <typename ValueType>
ValueType Stack<ValueType>::Peek() {
    bool ret;
    ValueType value;

    ret = _list.Get(_list.Size()-1,value);
    assert(ret);
    return value;
}

#endif //STACK_H
