#include <array>
#include <iostream>
#include <type_traits>

namespace constexpr_way {
    template <std::size_t N>
    consteval std::array<int, N> uniq(const std::array<int, N>& input) {
        std::array<int, N> output = {};
        std::size_t output_index = 0;
        int previous = input[0];
        output[output_index++] = previous;

        for (std::size_t i = 1; i < N; ++i) {
            int element = input[i];
            if (element != previous) {
                output[output_index++] = element;
                previous = element;
            }
        }

        // Fill remaining elements with a sentinel value or maintain the size
        for (std::size_t i = output_index; i < N; ++i) {
            output[i] = 0; // Assuming 0 as the sentinel value; adjust as necessary
        }

        return output;
    }
}

/*
We effectively build up the output vector by comparing the first two indices.

*/
namespace template_way {
    template <int... Nums>
    struct Vector;

    // Primary template
    template <typename InputVec, typename OutputVec = Vector<>>
    struct remove_dups;

    // when first and second are THE SAME
    template <int i, int... tail, int ... outElements>
    struct remove_dups<Vector<i,i,tail...>, Vector<outElements...>> {
        using type = remove_dups<Vector<i,tail...>, Vector<outElements...>>::type;
    };

    // when first and second are DIFFERENT
    template <int i, int... tail, int ... outElements>
    struct remove_dups<Vector<i,tail...>, Vector<outElements...>> {
        using type = remove_dups<Vector<tail...>, Vector<outElements...,i>>::type;
    };

    // base case (empty input)
    template <typename OutVector>
    struct remove_dups<Vector<>, OutVector> {
        using type = OutVector;
    };
}

int main() {
    constexpr std::array<int, 6> input = {1, 1, 2, 2, 3, 3};
    constexpr auto output = constexpr_way::uniq(input);

    for (int x : output) {
        if (x != 0) { // Only print non-zero values (or handle the sentinel appropriately)
            std::cout << x << ' ';
        }
    }

    using namespace template_way;

    // template way
    static_assert(std::is_same_v<Vector<1,2,3,4>, remove_dups<Vector<1,2,2,3,3,4,4,4,4>>::type>);

    return 0;
}
