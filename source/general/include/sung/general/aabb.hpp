#pragma once

#include <algorithm>

#include "mamath.hpp"
#include "optional.hpp"


namespace sung {

    // This class must allow integers as T
    template <typename T>
    class AABB1 {

    public:
        constexpr AABB1() = default;
        constexpr AABB1(T val0, T val1) {
            this->set(val0, val1);
        }

        constexpr T mini() const { return min_; }
        constexpr T maxi() const { return max_; }

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
            return sung::are_similiar(min_, rhs.min_, epsilon)
                && sung::are_similiar(max_, rhs.max_, epsilon);
        }

        constexpr bool is_intersecting_op(const AABB1& rhs) const {
            return max_ > rhs.min_ && min_ < rhs.max_;
        }
        constexpr bool is_intersecting_cl(const AABB1& rhs) const {
            return max_ >= rhs.min_ && min_ <= rhs.max_;
        }

        constexpr bool make_intersection(const AABB1& rhs, AABB1& output) const {
            if (!this->is_intersecting_cl(rhs))
                return false;

            output.min_ = (min_ > rhs.min_) ? min_ : rhs.min_;
            output.max_ = (max_ < rhs.max_) ? max_ : rhs.max_;
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
            }
            else {
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
        constexpr AABB2(T x0, T x1, T y0, T y1) {
            this->set(x0, x1, y0, y1);
        }

        constexpr T x_min() const { return x_.mini(); }
        constexpr T y_min() const { return y_.mini(); }
        constexpr T x_max() const { return x_.maxi(); }
        constexpr T y_max() const { return y_.maxi(); }

        constexpr T width() const { return x_.len(); }
        constexpr T height() const { return y_.len(); }
        constexpr T area() const {
            return this->width() * this->height();
        }

        constexpr bool are_similar(const AABB2& rhs, T epsilon) const {
            return x_.are_similar(rhs.x_, epsilon)
                && y_.are_similar(rhs.y_, epsilon);
        }

        constexpr bool is_inside_op(T x, T y) const {
            return x_.is_inside_op(x) && y_.is_inside_op(y);
        }
        constexpr bool is_inside_cl(T x, T y) const {
            return x_.is_inside_cl(x) && y_.is_inside_cl(y);
        }

        constexpr bool is_intersecting_op(const AABB2& rhs) const {
            return x_.is_intersecting_op(rhs.x_) && y_.is_intersecting_op(rhs.y_);
        }
        constexpr bool is_intersecting_cl(const AABB2& rhs) const {
            return x_.is_intersecting_cl(rhs.x_) && y_.is_intersecting_cl(rhs.y_);
        }

        constexpr bool make_intersection(const AABB2& rhs, AABB2& output) const {
            if (auto x_intersection = x_.make_intersection(rhs.x_)) {
                if (auto y_intersection = y_.make_intersection(rhs.y_)) {
                    output.x_ = *x_intersection;
                    output.y_ = *y_intersection;
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

}
