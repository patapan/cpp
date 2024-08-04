
// Check if a number is prime at compile time
// https://www.youtube.com/watch?v=cP1r8BonnuA&pp=ygUsdGVtcGxhdGUgbWV0YXByb2dyYW1taW5nIGludGVydmlldyBxdWVzdGlvbnM%3D

#include <type_traits>

namespace metaprogramming {

    // primary template
    template <int N, int M>
    struct is_prime_helper {
        static constexpr bool value = is_prime_helper<N,M-1>::value && (N%M != 0);
    };

    // partial specialization
    template <int N>
    struct is_prime_helper<N,1> {
        static constexpr bool value = true;
    };

    // primary template
    template <int T, typename = void>
    struct is_prime {
        static constexpr bool value = is_prime_helper<T,T-1>::value;
    };

    // sfinae
    template <int N>
    struct is_prime<N, std::enable_if_t<(N < 2)>> {
        constexpr bool static value = false;
    };

}

namespace concepts { 
    // primary template
    template <int N, int M>
    struct is_prime_helper {
        static constexpr bool value = is_prime_helper<N,M-1>::value && (N%M != 0);
    };

    // partial specialization
    template <int N>
    struct is_prime_helper<N,1> {
        static constexpr bool value = true;
    };

    // primary template
    template <int T>
    struct is_prime {
        static constexpr bool value = is_prime_helper<T,T-1>::value;
    };

    // template <int N>
    // concept LessThanTwo = (N < 2);

    // specialization for N<2
    template <int N>
    requires (N < 2)
    struct is_prime<N> {
        constexpr bool static value = false;
    };
}

namespace normalway {
    consteval bool is_prime(int i, int j) {
        if (j == 1) return true;
        if (i%j == 0) return false;
        return is_prime(i,j-1);
    }

    // First we do it the 'normal' way.
    consteval bool is_prime(int i) {
        if (i <= 1) return false;
        return is_prime(i,i-1);
    }
}


int main() {
    static_assert(normalway::is_prime(13),"");
    static_assert(!normalway::is_prime(10),"");

    static_assert(!metaprogramming::is_prime<1>::value, "");
    static_assert(!metaprogramming::is_prime<10>::value, "");
    static_assert(metaprogramming::is_prime<13>::value, "");

    static_assert(!concepts::is_prime<1>::value, "");
    static_assert(!concepts::is_prime<10>::value, "");
    static_assert(concepts::is_prime<13>::value, "");    
    
}