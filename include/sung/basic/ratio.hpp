#pragma once

#include <type_traits>


namespace sung {

    template <typename T>
    constexpr T gcd_rec(T a, T b) {
        static_assert(std::is_integral<T>::value, "T must be an integral type");
        return b == 0 ? a : gcd_rec(b, a % b);
    }


    template <typename T>
    class Ratio {

    public:
        constexpr Ratio(T num = 0, T den = 1) : num_(num), den_(den) {
            const auto g = gcd_rec(num_, den_);
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
