/*
WeakPtrs are used to break the recursive chain of 2 shared pointers pointing at eachother.

*/

#include <utility>

template <typename T>
class WeakPointer {
    T* data;
}
