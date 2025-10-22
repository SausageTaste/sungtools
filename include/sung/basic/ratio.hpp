#pragma once

#include <stdexcept>
#include <type_traits>


namespace sung { namespace internal {

    template <typename T>
    constexpr T gcd_rec(T a, T b) {
        static_assert(std::is_integral<T>::value, "T must be an integral type");
        return b == 0 ? a : gcd_rec(b, a % b);
    }

    template <typename T>
    constexpr T safe_abs(T x) {
        if (std::is_signed<T>::value)
            return x < 0 ? -x : x;
        else
            return x;
    }

}}  // namespace sung::internal


namespace sung {

    template <typename T>
    class Ratio {

    public:
        constexpr Ratio(T num = 0, T den = 1) : num_(num), den_(den) {
            if (0 == den_) {
                throw std::invalid_argument("Denominator cannot be zero");
            }

            if (std::is_signed<T>::value) {
                if (den_ < 0) {
                    num_ = -num_;
                    den_ = -den_;
                }
            }

            const auto g = internal::gcd_rec(
                internal::safe_abs(num_), internal::safe_abs(den_)
            );
            num_ /= g;
            den_ /= g;
        }

        constexpr T num() const { return num_; }

        constexpr T den() const { return den_; }

        template <typename U>
        constexpr U mul(U value) const {
            return value * static_cast<U>(num_) / static_cast<U>(den_);
        }

        template <typename U>
        constexpr U mul_rcp(U numerator) const {
            return numerator * static_cast<U>(den_) / static_cast<U>(num_);
        }

        template <typename U>
        constexpr U value() const {
            return static_cast<U>(num_) / static_cast<U>(den_);
        }

        template <typename U>
        constexpr U reciprocal() const {
            if (num_ == 0) {
                throw std::domain_error("Cannot take reciprocal of zero.");
            }

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

        constexpr bool operator==(const Ratio& rhs) const {
            return num_ == rhs.num_ && den_ == rhs.den_;
        }

        constexpr bool operator<(const Ratio& rhs) const {
            return num_ * rhs.den_ < rhs.num_ * den_;
        }

    private:
        T num_;
        T den_;
    };

}  // namespace sung
