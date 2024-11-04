
/*
A tuple is a fixed length container which can hold any type.

In the getter template, the index tells us how many times we need to split the head & rest.
*/
#include <cstdio>

// recursive case
template<typename First, typename... Rest>
struct Tuple: Tuple<Rest...> {
  Tuple(First first, Rest... rest): Tuple<Rest...>(rest...), first(first) {}

  First first;
};

// base case
template<typename First>
struct Tuple<First> {
  Tuple(First first): first(first) {}

  First first;
};

template<int index, typename First, typename... Rest>
struct Getter {
  static auto value(const Tuple<First, Rest...>& t) {
    return Getter<index - 1, Rest...>::value(t);
  }
};

template<typename First, typename... Rest>
struct Getter<0, First, Rest...> {
  static First value(const Tuple<First, Rest...>& t) {
    return t.first;
  }
};

template<int index, typename First, typename... Rest>
auto get(const Tuple<First, Rest...>& t) {
  return Getter<index, First, Rest...>::value(t);
}

int main() {
  Tuple<int, int, double> c(3, 5, 1.1);
  printf("%f\n", get<2>(c));
}