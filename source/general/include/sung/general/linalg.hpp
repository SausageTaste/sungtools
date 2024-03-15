#pragma once

#include <cstdint>

#include "mamath.hpp"


namespace sung {

    template <typename T>
    class TVec3;
    template <typename T>
    class TVec4;


    template <typename T>
    class TVec3 {

    public:
        constexpr TVec3() : elements_{ 0, 0, 0 } {}
        constexpr TVec3(T x, T y, T z) : elements_{ x, y, z } {}

        template <typename U>
        constexpr TVec3(const TVec4<U>& other)
            : elements_{ other.x(), other.y(), other.z() } {}

        // Element-wise operations
        constexpr TVec3 operator+(const TVec3& rhs) const {
            return TVec3{ this->x() + rhs.x(),
                          this->y() + rhs.y(),
                          this->z() + rhs.z() };
        }
        constexpr TVec3 operator-(const TVec3& rhs) const {
            return TVec3{ this->x() - rhs.x(),
                          this->y() - rhs.y(),
                          this->z() - rhs.z() };
        }
        constexpr TVec3 operator*(const TVec3& rhs) const {
            return TVec3{ this->x() * rhs.x(),
                          this->y() * rhs.y(),
                          this->z() * rhs.z() };
        }
        constexpr TVec3 operator/(const TVec3& rhs) const {
            return TVec3{ this->x() / rhs.x(),
                          this->y() / rhs.y(),
                          this->z() / rhs.z() };
        }

        constexpr TVec3& operator+=(const TVec3& rhs) {
            this->x() += rhs.x();
            this->y() += rhs.y();
            this->z() += rhs.z();
            return *this;
        }
        constexpr TVec3& operator-=(const TVec3& rhs) {
            this->x() -= rhs.x();
            this->y() -= rhs.y();
            this->z() -= rhs.z();
            return *this;
        }

        // Scalar operations
        constexpr TVec3 operator*(T rhs) const {
            return TVec3{ this->x() * rhs, this->y() * rhs, this->z() * rhs };
        }
        constexpr TVec3 operator/(T rhs) const {
            return TVec3{ this->x() / rhs, this->y() / rhs, this->z() / rhs };
        }

        // Unary operations
        constexpr TVec3 operator-() const {
            return TVec3{ -this->x(), -this->y(), -this->z() };
        }

        constexpr T& operator[](size_t i) { return elements_[i]; }
        constexpr T operator[](size_t i) const { return elements_[i]; }

        constexpr T& x() { return elements_[0]; }
        constexpr T& y() { return elements_[1]; }
        constexpr T& z() { return elements_[2]; }

        constexpr T x() const { return elements_[0]; }
        constexpr T y() const { return elements_[1]; }
        constexpr T z() const { return elements_[2]; }

        bool has_nan() const noexcept {
            return std::isnan(this->x()) || std::isnan(this->y()) ||
                   std::isnan(this->z());
        }
        bool has_inf() const noexcept {
            return std::isinf(this->x()) || std::isinf(this->y()) ||
                   std::isinf(this->z());
        }

        constexpr TVec3 lerp(const TVec3& rhs, T t) const {
            return *this + (rhs - *this) * t;
        }

        constexpr T dot(const TVec3& rhs) const {
            return (this->x() * rhs.x()) + (this->y() * rhs.y()) +
                   (this->z() * rhs.z());
        }

        constexpr TVec3 cross(const TVec3& rhs) const {
            return TVec3{ (this->y() * rhs.z()) - (this->z() * rhs.y()),
                          (this->z() * rhs.x()) - (this->x() * rhs.z()),
                          (this->x() * rhs.y()) - (this->y() * rhs.x()) };
        }

        constexpr T len_sqr() const { return this->dot(*this); }

        T len() const { return std::sqrt(this->len_sqr()); }

        constexpr T distance_sqr(const TVec3& rhs) const {
            return (*this - rhs).len_sqr();
        }

        T distance(const TVec3& rhs) const { return (*this - rhs).len(); }

        TVec3 normalize() const {
            const auto l = this->len();
            return TVec3{ this->x() / l, this->y() / l, this->z() / l };
        }

    private:
        T elements_[3];
    };


    template <typename T>
    constexpr T length(const TVec3<T>& v) {
        return v.len();
    }

    template <typename T>
    constexpr T dot(const TVec3<T>& a, const TVec3<T>& b) {
        return a.dot(b);
    }

    template <typename T>
    constexpr TVec3<T> cross(const TVec3<T>& a, const TVec3<T>& b) {
        return a.cross(b);
    }

    template <typename T>
    constexpr TVec3<T> normalize(const TVec3<T>& v) {
        return v.normalize();
    }


    template <typename T>
    class TVec4 {

    public:
        constexpr TVec4() : elements_{ 0, 0, 0, 0 } {}
        constexpr TVec4(T x, T y, T z, T w) : elements_{ x, y, z, w } {}

        template <typename U, typename V>
        constexpr TVec4(const TVec3<U>& v, V w)
            : elements_{ static_cast<T>(v.x()),
                         static_cast<T>(v.y()),
                         static_cast<T>(v.z()),
                         static_cast<T>(w) } {}

        template <typename U>
        constexpr TVec4(const TVec4<U>& other)
            : elements_{ static_cast<T>(other.x()),
                         static_cast<T>(other.y()),
                         static_cast<T>(other.z()),
                         static_cast<T>(other.w()) } {}

        // Element-wise operations
        constexpr TVec4 operator+(const TVec4& rhs) const {
            return TVec4{ this->x() + rhs.x(),
                          this->y() + rhs.y(),
                          this->z() + rhs.z(),
                          this->w() + rhs.w() };
        }
        constexpr TVec4 operator-(const TVec4& rhs) const {
            return TVec4{ this->x() - rhs.x(),
                          this->y() - rhs.y(),
                          this->z() - rhs.z(),
                          this->w() - rhs.w() };
        }
        constexpr TVec4 operator*(const TVec4& rhs) const {
            return TVec4{ this->x() * rhs.x(),
                          this->y() * rhs.y(),
                          this->z() * rhs.z(),
                          this->w() * rhs.w() };
        }
        constexpr TVec4 operator/(const TVec4& rhs) const {
            return TVec4{ this->x() / rhs.x(),
                          this->y() / rhs.y(),
                          this->z() / rhs.z(),
                          this->w() / rhs.w() };
        }

        constexpr TVec4& operator+=(const TVec4& rhs) {
            this->x() += rhs.x();
            this->y() += rhs.y();
            this->z() += rhs.z();
            this->w() += rhs.w();
            return *this;
        }
        constexpr TVec4& operator-=(const TVec4& rhs) {
            this->x() -= rhs.x();
            this->y() -= rhs.y();
            this->z() -= rhs.z();
            this->w() -= rhs.w();
            return *this;
        }

        // Scalar operations
        constexpr TVec4 operator*(T rhs) const {
            return TVec4{ this->x() * rhs,
                          this->y() * rhs,
                          this->z() * rhs,
                          this->w() * rhs };
        }
        constexpr TVec4 operator/(T rhs) const {
            return TVec4{ this->x() / rhs,
                          this->y() / rhs,
                          this->z() / rhs,
                          this->w() / rhs };
        }

        // Unary operations
        constexpr TVec4 operator-() const {
            return TVec4{ -this->x(), -this->y(), -this->z(), -this->w() };
        }

        constexpr T& operator[](size_t i) { return elements_[i]; }
        constexpr T operator[](size_t i) const { return elements_[i]; }

        constexpr T& x() { return elements_[0]; }
        constexpr T& y() { return elements_[1]; }
        constexpr T& z() { return elements_[2]; }
        constexpr T& w() { return elements_[3]; }

        constexpr T x() const { return elements_[0]; }
        constexpr T y() const { return elements_[1]; }
        constexpr T z() const { return elements_[2]; }
        constexpr T w() const { return elements_[3]; }

        constexpr bool are_similar(const TVec4& rhs, T epsilon) const {
            return sung::are_similiar(this->x(), rhs.x(), epsilon) &&
                   sung::are_similiar(this->y(), rhs.y(), epsilon) &&
                   sung::are_similiar(this->z(), rhs.z(), epsilon) &&
                   sung::are_similiar(this->w(), rhs.w(), epsilon);
        }

        constexpr T dot(const TVec4& rhs) const {
            return (this->x() * rhs.x()) + (this->y() * rhs.y()) +
                   (this->z() * rhs.z()) + (this->w() * rhs.w());
        }

        constexpr T len_sqr() const { return this->dot(*this); }

        T len() const { return std::sqrt(this->len_sqr()); }

        constexpr T distance_sqr(const TVec4& rhs) const {
            return (*this - rhs).len_sqr();
        }

        T distance(const TVec4& rhs) const { return (*this - rhs).len(); }

        TVec4 normalize() const {
            const auto l = this->len();
            return TVec4{
                this->x() / l, this->y() / l, this->z() / l, this->w() / l
            };
        }

    private:
        T elements_[4];
    };


    template <typename T>
    class TMat4 {

    public:
        constexpr TMat4() = default;

        constexpr static TMat4 identity() {
            TMat4 m;
            m.at(0, 0) = 1;
            m.at(1, 1) = 1;
            m.at(2, 2) = 1;
            m.at(3, 3) = 1;
            return m;
        }

        constexpr static TMat4 translate(T x, T y, T z) {
            auto m = TMat4::identity();
            m.at(0, 3) = x;
            m.at(1, 3) = y;
            m.at(2, 3) = z;
            return m;
        }

        static TMat4 rotate_axis(TVec3<T> axis, T angle) {
            axis = axis.normalize();

            const auto c = std::cos(angle);
            const auto s = std::sin(angle);
            const auto t = static_cast<T>(1) - c;
            const auto x = axis.x();
            const auto y = axis.y();
            const auto z = axis.z();

            TMat4 output;
            output.elements_[0][0] = t * x * x + c;
            output.elements_[0][1] = t * x * y - z * s;
            output.elements_[0][2] = t * x * z + y * s;

            output.elements_[1][0] = t * x * y + z * s;
            output.elements_[1][1] = t * y * y + c;
            output.elements_[1][2] = t * y * z - x * s;

            output.elements_[2][0] = t * x * z - y * s;
            output.elements_[2][1] = t * y * z + x * s;
            output.elements_[2][2] = t * z * z + c;
            return output;
        }

        constexpr T& at(size_t row, size_t col) { return elements_[row][col]; }
        constexpr T at(size_t row, size_t col) const {
            return elements_[row][col];
        }
        constexpr TVec4<T>& row(size_t r) { return elements_[r]; }
        constexpr const TVec4<T>& row(size_t r) const { return elements_[r]; }

        constexpr TVec4<T> column(size_t c) const {
            return TVec4<T>{
                this->at(0, c), this->at(1, c), this->at(2, c), this->at(3, c)
            };
        }

    private:
        TVec4<T> elements_[4];
    };

    static_assert(
        sizeof(TMat4<double>) == sizeof(double) * 16,
        "TMat4<double> must be as big as 4 doubles"
    );


    template <typename T>
    constexpr TMat4<T> operator*(const TMat4<T>& a, const TMat4<T>& b) {
        TMat4<T> result;
        for (size_t r = 0; r < 4; ++r) {
            for (size_t c = 0; c < 4; ++c) {
                result.at(r, c) = a.row(r).dot(b.column(c));
            }
        }
        return result;
    }


    template <typename T>
    constexpr TVec4<T> operator*(const TMat4<T>& m, const TVec4<T>& v) {
        return TVec4<T>{
            m.row(0).dot(v), m.row(1).dot(v), m.row(2).dot(v), m.row(3).dot(v)
        };
    }

}  // namespace sung
