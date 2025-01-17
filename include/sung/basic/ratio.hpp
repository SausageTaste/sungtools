#pragma once

#include <numeric>
#include <type_traits>


namespace sung {

    template <class M, class N>
    constexpr std::common_type_t<M, N> gcd(M a, N b) {
#ifdef __cpp_lib_gcd_lcm
        return std::gcd(a, b);
#else
        static_assert(std::is_integral<M>::value, "M must be an integral type");
        static_assert(std::is_integral<N>::value, "N must be an integral type");

        while (b != 0) {
            const auto t = b;
            b = a % b;
            a = t;
        }
        return a;
#endif
    }


    template <typename T>
    class Ratio {

    public:
        constexpr Ratio(T num = 0, T den = 1) : num_(num), den_(den) {
            const auto g = gcd(num_, den_);
            num_ /= g;
            den_ /= g;
        }

        constexpr T num() const { return num_; }

        constexpr T den() const { return den_; }

        template <typename U>
        constexpr U value() const {
            return static_cast<U>(num_) / static_cast<U>(den_);
        }

        template <typename U>
        constexpr U reciprocal() const {
            return static_cast<U>(den_) / static_cast<U>(num_);
        }

        constexpr Ratio operator+(const Ratio& rhs) const {
            return Ratio(num_ * rhs.den_ + rhs.num_ * den_, den_ * rhs.den_);
        }

        constexpr Ratio operator-(const Ratio& rhs) const {
            return Ratio(num_ * rhs.den_ - rhs.num_ * den_, den_ * rhs.den_);
        }

        constexpr Ratio operator*(const Ratio& rhs) const {
            return Ratio(num_ * rhs.num_, den_ * rhs.den_);
        }

        constexpr Ratio operator/(const Ratio& rhs) const {
            return Ratio(num_ * rhs.den_, den_ * rhs.num_);
        }

    private:
        T num_;
        T den_;
    };

}  // namespace sung
