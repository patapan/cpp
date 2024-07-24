
/*
Given a vector
template <int...I> struct Vector;

Assume the vector is sorted. Remove adjacent duplicates from the vector.

The idea:
- Using variadic templating
*/

template <int...I>
struct Vector;

template <Vector V, int i>
struct helper {
    constexpr static Vector value = 
}

template <Vector V, Vector OutputV = Vector<>>
struct remove_duplicates {
    constexpr static Vector value = helper<V,0>;
}



int main() {
    static_assert(remove_duplicates<Vector<1,2,3> == remove_duplicates<Vector<1,1,2,2,2,3>);
}