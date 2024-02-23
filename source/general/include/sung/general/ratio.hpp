#pragma once


namespace sung {

    template <typename T>
    class Ratio {

    public:
        constexpr Ratio(T num = 0, T den = 1) : num_(num), den_(den) {}

        constexpr T num() const {
            return num_;
        }

        constexpr T den() const {
            return den_;
        }

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


    void test() {
        constexpr Ratio<int> r0(5, 7);
        constexpr Ratio<int> r1(3, 4);
        constexpr auto r2 = r0 * r1;
        constexpr auto r3 = r0 / r1;
        constexpr auto r4 = r0 + r1;
        constexpr auto r5 = r0 - r1;
    }

}
