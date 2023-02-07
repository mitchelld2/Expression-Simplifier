//
// Interface Definition for the template version of the VariableArrayList Class
// Author: Max Benson
// Date: 08/07/2021
//

#ifndef VARIABLEARRAYLIST_H
#define VARIABLEARRAYLIST_H

#include <assert.h>
#include <iostream>
using std::ostream;

template <typename ItemType>
class VariableArrayList  {
public:
    VariableArrayList();   //Default constructor
    ~VariableArrayList();

    VariableArrayList(const VariableArrayList&);
    const VariableArrayList& operator=(const VariableArrayList&);

    bool Insert(size_t position, const ItemType& item);
    int Find(const ItemType& item, size_t start = 0) const;
    bool Remove(size_t position, ItemType& item);
    bool Get(size_t position, ItemType& item) const;
    void Clear();

    size_t Size() const;
    size_t Capacity() const;

    bool CheckConsistency() const;

    friend ostream& operator<<(ostream& os, const VariableArrayList& list) {
        os << "[";
        for (size_t position = 0; position < list._size; position ++) {
            if (position > 0) {
                os << ", ";
            }
            os << list._array[position];
        }
        return os << "]";
    }

private:
    bool GrowCapacity();
    void ShrinkCapacity();

    ItemType* _array;
    size_t _size;
    size_t _capacity;
};

/**
 * Default constructor
 * Creates an empty list.
 */
template <typename ItemType>
VariableArrayList<ItemType>::VariableArrayList() {
    _array = new ItemType[1];
    _size = 0;
    _capacity = 1;
}

/**
 * Destructor
 * Frees the dynamic memory allocated for the list
 */
template <typename ItemType>
VariableArrayList<ItemType>::~VariableArrayList() {
    delete[] _array;
}

/**
 * Copy Constructor
 * Creates a deep copy of the list.
 * The running time of the whole method is O(N) time where N is the
 * length of the list
 * @param other the list to be copied
 */
template <typename ItemType>
VariableArrayList<ItemType>::VariableArrayList(const VariableArrayList& other) {
    _array = new ItemType[other._capacity];
    for (size_t i = 0; i < other._size; i ++) {
        _array[i] = other._array[i];
    }
    _size = other._size;
    _capacity = other._capacity;
}

/**
 * Copy assignment operator
 * Enables deep copy assignment using the operator = overload.
 * Uses the copy constructor to copy the rhs.  Then swaps internals of
 * the copy with this.  By doing it this way, the old contents
 * of this will be automatically freed when the method finishes.
 * The running time of this method is the same as the copy
 * constructor , i.e. O(N)
 * @param rhs the object to be copied into this
 * @return this to enable cascade assignments
 */
template <typename ItemType>
const VariableArrayList<ItemType>& VariableArrayList<ItemType>::operator=(const VariableArrayList& rhs) {
    if (this != &rhs) {
        VariableArrayList copy(rhs);
        ItemType* swapArray;
        size_t swapSize;
        size_t swapCapacity;

        swapArray = _array;
        swapSize = _size;
        swapCapacity = _capacity;
        _array = copy._array;
        _capacity = copy._capacity;
        _size = copy._size;
        copy._array = swapArray;
        copy._size = swapSize;
        copy._capacity = swapCapacity;
    }
    return *this;
}

/**
 * Inserts an element into a given position so long as the position is valid
 * and the list is not filled to capacity.
 * THe running time of inserting at the start is O(N), it's O(1) if you are
 * inserting at the end.
 * @param item what the client wants to insert into the list
 * @param position the position where the element is to be inserted
 * @return true if it was possible to insert, false otherwise.
 */
template <typename ItemType>
bool VariableArrayList<ItemType>::Insert(size_t position, const ItemType& item) {
    if (position > _size) {
        return false;
    }
    else {
        if (_size == _capacity && !GrowCapacity()) {
            return false;
        }

        // Shift out elements to make room for insertion
        for (size_t i = _size; i > position; i --) {
            _array[i] = _array[i-1];
        }
        // Insert the element
        _array[position] = item;
        _size ++;
        return true;
    }
}

/**
 * Searches for the position of the first matching item in the list starting
 * at position "start".  The method performs a linear search for the item.
 * If the element is found it returns the position, if it is not found it
 * returns -1.
 * The running time of this method depends on where the search starts
 * and where the item is found (if it is found)
 * @param item the object that the client is searching for.
 * @param start position at which to start the search
 * @return the position of the element if found, -1 otherwise.
 */
template <typename ItemType>
int VariableArrayList<ItemType>::Find(const ItemType& item, size_t start)  const {
    for (size_t i = start; i < _size; i++) {
        if (_array[i] == item) {
            return i;
        }
    }
    return -1;
}

/**
 * Removes the item at position, so long as the position is valid. The item previously
 * stored in the list is returned in the supplied parameter.
 * THe running time of removing the first element is  O(N), it's O(1) if you are
 * removing the last element.
 * @param position the position of the element to be removed.
 * @param item the item previously stored in the list
 * @return true if node could be deleted, false if position at end of list or invalid,
 */
template <typename ItemType>
bool VariableArrayList<ItemType>::Remove(size_t position, ItemType& item) {
    if (position >= _size) {
        return false;
    }
    else {
        item = _array[position];

        // Close up the gap
        for (size_t i = position; i < _size-1; i ++) {
            _array[i] = _array[i+1];
        }
        _size--;

        ShrinkCapacity();

        return true;
    }
}

/**
 * Get  the item at a specified position
 * This method runs in O(1) time.
 * @param position the position of the element to retrieve.
 * @param item the item found at position
 * @return true if valid position given, false otherwise
 */
template <typename ItemType>
bool VariableArrayList<ItemType>::Get(size_t position, ItemType& item) const {
    if (position < _size) {
        item = _array[position];
        return true;
    }
    return false;
}

/**
 * Clear the list
 * and then resetting _head to nullptr.
 * This method runs in O(1) time
 * There is no need to free the string values
 * that will occur when the class destructs
 */
template <typename ItemType>
void VariableArrayList<ItemType>::Clear() {
    _size = 0;
}

/**
 * Returns the current number of items in list
 * This method runs in O(1) time
 * @return current number of itesm in list
 */
template <typename ItemType>
size_t VariableArrayList<ItemType>::Size() const {
    return _size;
}

/**
 * Returns the current allocated size of the array
 * This method runs in O(1) time
 * @return current allocated size of array
 */
template <typename ItemType>
size_t VariableArrayList<ItemType>::Capacity() const {
    return _capacity;
}

/**
 * Checks if list data structure appears to be consistent
 * @return true if list consistent, false otherwise
 */
template <typename ItemType>
bool VariableArrayList<ItemType>::CheckConsistency() const {
    return _size <= _capacity;
}

/**
 * Doubles the size of the array allocation
 * This is meant to be called when array is full.
 * @return success
 */
template <typename ItemType>
bool VariableArrayList<ItemType>::GrowCapacity() {
    ItemType* newArray;

    assert(_size == _capacity);
    newArray = new ItemType[2*_capacity];
    for (size_t i = 0; i < _size; i ++) {
        newArray[i] = _array[i];
    }
    delete[] _array;
    _array = newArray;
    _capacity *= 2;
    return true;
}

/**
 * Check if array allocation should be reduced
 * If using less than 1/4 capacity, halves the size of allocation.
 */
template <typename ItemType>
void VariableArrayList<ItemType>::ShrinkCapacity() {
    if (_size > 0 && 4*_size <= _capacity) {
        ItemType* newArray;

        newArray = new ItemType[_capacity/2];
        for (size_t i = 0; i < _size; i ++) {
            newArray[i] = _array[i];
        }
        delete[] _array;
        _array = newArray;
        _capacity /= 2;
    }
}

#endif //VARIABLEARRAYLIST_H
