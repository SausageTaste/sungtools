#pragma once

#include <algorithm>

#include "linalg.hpp"
#include "mamath.hpp"
#include "optional.hpp"


namespace sung {

    // This class must allow integers as T
    template <typename T>
    class AABB1 {

    public:
        constexpr AABB1() = default;
        constexpr AABB1(T val0, T val1) { this->set(val0, val1); }

        constexpr AABB1 operator+(T val) const {
            return AABB1{ min_ + val, max_ + val };
        }
        constexpr AABB1 operator-(T val) const {
            return AABB1{ min_ - val, max_ - val };
        }
        constexpr AABB1& operator+=(T val) {
            min_ += val;
            max_ += val;
            return *this;
        }
        constexpr AABB1& operator-=(T val) {
            min_ -= val;
            max_ -= val;
            return *this;
        }

        constexpr T mini() const { return min_; }
        constexpr T maxi() const { return max_; }

        constexpr T mid() const { return (min_ + max_) / 2; }
        constexpr T len() const { return max_ - min_; }

        // Open interval
        constexpr bool is_inside_op(T val) const {
            return val > min_ && val < max_;
        }
        // Closed interval
        constexpr bool is_inside_cl(T val) const {
            return val >= min_ && val <= max_;
        }

        constexpr bool are_similar(const AABB1& rhs, T epsilon) const {
            return sung::are_similiar(min_, rhs.min_, epsilon) &&
                   sung::are_similiar(max_, rhs.max_, epsilon);
        }

        constexpr bool is_intersecting_op(const AABB1& rhs) const {
            return max_ > rhs.min_ && min_ < rhs.max_;
        }
        constexpr bool is_intersecting_cl(const AABB1& rhs) const {
            return max_ >= rhs.min_ && min_ <= rhs.max_;
        }

        constexpr bool make_intersection(const AABB1& rhs, AABB1& out) const {
            if (!this->is_intersecting_cl(rhs))
                return false;

            out.min_ = (min_ > rhs.min_) ? min_ : rhs.min_;
            out.max_ = (max_ < rhs.max_) ? max_ : rhs.max_;
            return true;
        }

        constexpr Optional<AABB1> make_intersection(const AABB1& rhs) const {
            AABB1 output;
            if (this->make_intersection(rhs, output))
                return output;
            else
                return sung::nullopt;
        }

        // It makes the length 0
        constexpr void set(T val) {
            min_ = val;
            max_ = val;
        }
        constexpr void set(T val0, T val1) {
            if (val0 < val1) {
                min_ = val0;
                max_ = val1;
            } else {
                min_ = val1;
                max_ = val0;
            }
        }

        constexpr void expand_to_span(T val) {
            if (min_ > val)
                min_ = val;
            if (max_ < val)
                max_ = val;
        }
        constexpr AABB1 get_expanded_to_span(T val) const {
            AABB1 output = *this;
            output.expand_to_span(val);
            return output;
        }

    private:
        T min_{};
        T max_{};
    };


    // This class must allow integers as T
    template <typename T>
    class AABB2 {

    public:
        constexpr AABB2() = default;
        constexpr AABB2(T x0, T x1, T y0, T y1) { this->set(x0, x1, y0, y1); }

        constexpr T x_min() const { return x_.mini(); }
        constexpr T x_max() const { return x_.maxi(); }
        constexpr T y_min() const { return y_.mini(); }
        constexpr T y_max() const { return y_.maxi(); }

        constexpr T x_mid() const { return x_.mid(); }
        constexpr T y_mid() const { return y_.mid(); }
        constexpr T x_len() const { return x_.len(); }
        constexpr T y_len() const { return y_.len(); }

        constexpr T width() const { return x_.len(); }
        constexpr T height() const { return y_.len(); }
        constexpr T area() const { return this->width() * this->height(); }

        constexpr bool are_similar(const AABB2& rhs, T epsilon) const {
            return x_.are_similar(rhs.x_, epsilon) &&
                   y_.are_similar(rhs.y_, epsilon);
        }

        constexpr bool is_inside_op(T x, T y) const {
            return x_.is_inside_op(x) && y_.is_inside_op(y);
        }
        constexpr bool is_inside_cl(T x, T y) const {
            return x_.is_inside_cl(x) && y_.is_inside_cl(y);
        }

        constexpr bool is_intersecting_op(const AABB2& rhs) const {
            return x_.is_intersecting_op(rhs.x_) &&
                   y_.is_intersecting_op(rhs.y_);
        }
        constexpr bool is_intersecting_cl(const AABB2& rhs) const {
            return x_.is_intersecting_cl(rhs.x_) &&
                   y_.is_intersecting_cl(rhs.y_);
        }

        constexpr bool make_intersection(const AABB2& rhs, AABB2& out) const {
            if (auto x_intersection = x_.make_intersection(rhs.x_)) {
                if (auto y_intersection = y_.make_intersection(rhs.y_)) {
                    out.x_ = *x_intersection;
                    out.y_ = *y_intersection;
                    return true;
                }
            }

            return false;
        }
        constexpr Optional<AABB2> make_intersection(const AABB2& rhs) const {
            AABB2 output;
            if (this->make_intersection(rhs, output))
                return output;
            else
                return sung::nullopt;
        }

        // Yes it makes the volume 0
        constexpr void set(T x, T y) {
            x_.set(x);
            y_.set(y);
        }
        constexpr void set(T x0, T x1, T y0, T y1) {
            x_.set(x0, x1);
            y_.set(y0, y1);
        }

        constexpr void expand_to_span(T x, T y) {
            x_.expand_to_span(x);
            y_.expand_to_span(y);
        }
        constexpr AABB2 get_expanded_to_span(T x, T y) const {
            AABB2 output = *this;
            output.expand_to_span(x, y);
            return output;
        }

    private:
        AABB1<T> x_, y_;
    };


    template <typename T>
    class AABB3 {

    public:
        constexpr AABB3() = default;
        constexpr AABB3(T x0, T x1, T y0, T y1, T z0, T z1) {
            this->set(x0, x1, y0, y1, z0, z1);
        }
        constexpr AABB3(const TVec3<T>& min, const TVec3<T>& max) {
            this->set(min, max);
        }

        constexpr AABB3 operator+(const TVec3<T>& v) const {
            return AABB3{ x_ + v.x(), y_ + v.y(), z_ + v.z() };
        }
        constexpr AABB3 operator-(const TVec3<T>& v) const {
            return AABB3{ x_ - v.x(), y_ - v.y(), z_ - v.z() };
        }
        constexpr AABB3& operator+=(const TVec3<T>& v) {
            x_ += v.x();
            y_ += v.y();
            z_ += v.z();
            return *this;
        }
        constexpr AABB3& operator-=(const TVec3<T>& v) {
            x_ -= v.x();
            y_ -= v.y();
            z_ -= v.z();
            return *this;
        }

        constexpr T x_min() const { return x_.mini(); }
        constexpr T x_max() const { return x_.maxi(); }
        constexpr T y_min() const { return y_.mini(); }
        constexpr T y_max() const { return y_.maxi(); }
        constexpr T z_min() const { return z_.mini(); }
        constexpr T z_max() const { return z_.maxi(); }

        constexpr TVec3<T> mini() const {
            return TVec3<T>{ this->x_min(), this->y_min(), this->z_min() };
        }
        constexpr TVec3<T> maxi() const {
            return TVec3<T>{ this->x_max(), this->y_max(), this->z_max() };
        }

        constexpr T x_mid() const { return x_.mid(); }
        constexpr T y_mid() const { return y_.mid(); }
        constexpr T z_mid() const { return z_.mid(); }
        constexpr T x_len() const { return x_.len(); }
        constexpr T y_len() const { return y_.len(); }
        constexpr T z_len() const { return z_.len(); }

        constexpr T volume() const {
            return this->x_len() * this->y_len() * this->z_len();
        }

        constexpr bool are_similar(const AABB3& rhs, T epsilon) const {
            return x_.are_similar(rhs.x_, epsilon) &&
                   y_.are_similar(rhs.y_, epsilon) &&
                   z_.are_similar(rhs.z_, epsilon);
        }

        constexpr bool is_inside_op(T x, T y, T z) const {
            return x_.is_inside_op(x) && y_.is_inside_op(y) &&
                   z_.is_inside_op(z);
        }
        constexpr bool is_inside_op(const TVec3<T>& point) const {
            return this->is_inside_op(point.x(), point.y(), point.z());
        }
        constexpr bool is_inside_cl(T x, T y, T z) const {
            return x_.is_inside_cl(x) && y_.is_inside_cl(y) &&
                   z_.is_inside_cl(z);
        }
        constexpr bool is_inside_cl(const TVec3<T>& point) const {
            return this->is_inside_cl(point.x(), point.y(), point.z());
        }

        constexpr bool is_intersecting_op(const AABB3& rhs) const {
            return x_.is_intersecting_op(rhs.x_) &&
                   y_.is_intersecting_op(rhs.y_) &&
                   z_.is_intersecting_op(rhs.z_);
        }
        constexpr bool is_intersecting_cl(const AABB3& rhs) const {
            return x_.is_intersecting_cl(rhs.x_) &&
                   y_.is_intersecting_cl(rhs.y_) &&
                   z_.is_intersecting_cl(rhs.z_);
        }

        constexpr bool make_intersection(const AABB3& rhs, AABB3& out) const {
            if (auto x_intersection = x_.make_intersection(rhs.x_)) {
                if (auto y_intersection = y_.make_intersection(rhs.y_)) {
                    if (auto z_intersection = z_.make_intersection(rhs.z_)) {
                        out.x_ = *x_intersection;
                        out.y_ = *y_intersection;
                        out.z_ = *z_intersection;
                        return true;
                    }
                }
            }

            return false;
        }
        constexpr Optional<AABB3> make_intersection(const AABB3& rhs) const {
            AABB3 output;
            if (this->make_intersection(rhs, output))
                return output;
            else
                return sung::nullopt;
        }

        constexpr void set(T x, T y, T z) {
            x_.set(x);
            y_.set(y);
            z_.set(y);
        }
        constexpr void set(const TVec3<T>& point) {
            this->set(point.x(), point.y(), point.z());
        }
        constexpr void set(T x0, T x1, T y0, T y1, T z0, T z1) {
            x_.set(x0, x1);
            y_.set(y0, y1);
            z_.set(z0, z1);
        }
        constexpr void set(const TVec3<T>& min, const TVec3<T>& max) {
            this->set(min.x(), max.x(), min.y(), max.y(), min.z(), max.z());
        }

        constexpr void expand_to_span(T x, T y, T z) {
            x_.expand_to_span(x);
            y_.expand_to_span(y);
            z_.expand_to_span(z);
        }
        constexpr void expand_to_span(const TVec3<T>& point) {
            this->expand_to_span(point.x(), point.y(), point.z());
        }
        constexpr AABB3 get_expanded_to_span(T x, T y, T z) const {
            AABB3 output = *this;
            output.expand_to_span(x, y, z);
            return output;
        }
        constexpr AABB3 get_expanded_to_span(const TVec3<T>& point) const {
            return this->get_expanded_to_span(point.x(), point.y(), point.z());
        }

    private:
        AABB1<T> x_, y_, z_;
    };

}  // namespace sung
