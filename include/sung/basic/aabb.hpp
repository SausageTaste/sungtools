#pragma once

#include <algorithm>

#include "linalg.hpp"
#include "mamath.hpp"
#include "optional.hpp"


namespace sung { namespace internal {

    // This class must allow integers as T
    template <typename T>
    class Aabb1DBase {

    public:
        using val_t = T;

        constexpr Aabb1DBase() = default;
        constexpr Aabb1DBase(T val0, T val1) { this->set(val0, val1); }

        constexpr Aabb1DBase operator+(T val) const {
            return Aabb1DBase{ min_ + val, max_ + val };
        }
        constexpr Aabb1DBase operator-(T val) const {
            return Aabb1DBase{ min_ - val, max_ - val };
        }
        constexpr Aabb1DBase& operator+=(T val) {
            min_ += val;
            max_ += val;
            return *this;
        }
        constexpr Aabb1DBase& operator-=(T val) {
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

        constexpr T remap_linear(T val, T new_min, T new_max) const {
            return new_min + (val - min_) * (new_max - new_min) / (max_ - min_);
        }
        constexpr T remap_linear(T val) const {
            return (val - min_) / (max_ - min_);
        }

        constexpr bool are_similar(const Aabb1DBase& rhs, T epsilon) const {
            return sung::are_similiar(min_, rhs.min_, epsilon) &&
                   sung::are_similiar(max_, rhs.max_, epsilon);
        }

        constexpr bool is_intersecting_op(const Aabb1DBase& rhs) const {
            return max_ > rhs.min_ && min_ < rhs.max_;
        }
        constexpr bool is_intersecting_cl(const Aabb1DBase& rhs) const {
            return max_ >= rhs.min_ && min_ <= rhs.max_;
        }

        constexpr bool make_intersection(
            const Aabb1DBase& rhs, Aabb1DBase& out
        ) const {
            if (!this->is_intersecting_cl(rhs))
                return false;

            out.min_ = (min_ > rhs.min_) ? min_ : rhs.min_;
            out.max_ = (max_ < rhs.max_) ? max_ : rhs.max_;
            return true;
        }

        constexpr Optional<Aabb1DBase> make_intersection(
            const Aabb1DBase& rhs
        ) const {
            Aabb1DBase output;
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
        constexpr Aabb1DBase get_expanded_to_span(T val) const {
            Aabb1DBase output = *this;
            output.expand_to_span(val);
            return output;
        }

        constexpr void offset(T val) {
            min_ += val;
            max_ += val;
        }

    private:
        T min_{};
        T max_{};
    };


    template <typename TAabb1D>
    class Aabb2DBase {

    public:
        using val_t = typename TAabb1D::val_t;
        using Vec2 = TVec2<val_t>;
        using OptSelf = Optional<Aabb2DBase>;

        constexpr Aabb2DBase() = default;
        constexpr Aabb2DBase(val_t x0, val_t x1, val_t y0, val_t y1) {
            this->set(x0, x1, y0, y1);
        }

        constexpr val_t x_min() const { return x_.mini(); }
        constexpr val_t x_max() const { return x_.maxi(); }
        constexpr val_t y_min() const { return y_.mini(); }
        constexpr val_t y_max() const { return y_.maxi(); }

        constexpr val_t x_mid() const { return x_.mid(); }
        constexpr val_t y_mid() const { return y_.mid(); }
        constexpr val_t x_len() const { return x_.len(); }
        constexpr val_t y_len() const { return y_.len(); }

        constexpr val_t width() const { return x_.len(); }
        constexpr val_t height() const { return y_.len(); }
        constexpr val_t area() const { return this->width() * this->height(); }

        constexpr bool are_similar(const Aabb2DBase& rhs, val_t epsilon) const {
            return x_.are_similar(rhs.x_, epsilon) &&
                   y_.are_similar(rhs.y_, epsilon);
        }

        constexpr bool is_inside_op(val_t x, val_t y) const {
            return x_.is_inside_op(x) && y_.is_inside_op(y);
        }
        constexpr bool is_inside_cl(val_t x, val_t y) const {
            return x_.is_inside_cl(x) && y_.is_inside_cl(y);
        }

        constexpr bool is_intersecting_op(const Aabb2DBase& rhs) const {
            return x_.is_intersecting_op(rhs.x_) &&
                   y_.is_intersecting_op(rhs.y_);
        }
        constexpr bool is_intersecting_cl(const Aabb2DBase& rhs) const {
            return x_.is_intersecting_cl(rhs.x_) &&
                   y_.is_intersecting_cl(rhs.y_);
        }

        constexpr double closest_distance(const Vec2& point) const {
            double dx = 0.0;
            if (point.x() < x_min())
                dx = x_min() - point.x();
            else if (point.x() > x_max())
                dx = point.x() - x_max();

            double dy = 0.0;
            if (point.y() < y_min())
                dy = y_min() - point.y();
            else if (point.y() > y_max())
                dy = point.y() - y_max();

            return std::sqrt(dx * dx + dy * dy);
        }

        constexpr bool make_intersection(
            const Aabb2DBase& rhs, Aabb2DBase& out
        ) const {
            if (auto x_intersection = x_.make_intersection(rhs.x_)) {
                if (auto y_intersection = y_.make_intersection(rhs.y_)) {
                    out.x_ = *x_intersection;
                    out.y_ = *y_intersection;
                    return true;
                }
            }

            return false;
        }

        constexpr OptSelf make_intersection(const Aabb2DBase& rhs) const {
            Aabb2DBase output;
            if (this->make_intersection(rhs, output))
                return output;
            else
                return sung::nullopt;
        }

        constexpr void set_x(val_t x0, val_t x1) { x_.set(x0, x1); }
        constexpr void set_y(val_t y0, val_t y1) { y_.set(y0, y1); }

        // Yes it makes the volume 0
        constexpr void set(val_t x, val_t y) {
            x_.set(x);
            y_.set(y);
        }
        constexpr void set(val_t x0, val_t x1, val_t y0, val_t y1) {
            x_.set(x0, x1);
            y_.set(y0, y1);
        }

        constexpr void expand_to_span(val_t x, val_t y) {
            x_.expand_to_span(x);
            y_.expand_to_span(y);
        }
        constexpr Aabb2DBase get_expanded_to_span(val_t x, val_t y) const {
            Aabb2DBase output = *this;
            output.expand_to_span(x, y);
            return output;
        }

        constexpr void offset(val_t x, val_t y) {
            x_.offset(x);
            y_.offset(y);
        }

        constexpr void offset(const Vec2& v) {
            x_.offset(v.x());
            y_.offset(v.y());
        }

    protected:
        TAabb1D x_, y_;
    };


    template <typename TAabb1D>
    class Aabb3DBase {

    public:
        using val_t = typename TAabb1D::val_t;
        using Vec3 = TVec3<val_t>;
        using OptSelf = Optional<Aabb3DBase>;

        constexpr Aabb3DBase() = default;
        constexpr Aabb3DBase(
            val_t x0, val_t x1, val_t y0, val_t y1, val_t z0, val_t z1
        ) {
            this->set(x0, x1, y0, y1, z0, z1);
        }
        constexpr Aabb3DBase(const Vec3& min, const Vec3& max) {
            this->set(min, max);
        }

        constexpr val_t x_min() const { return x_.mini(); }
        constexpr val_t x_max() const { return x_.maxi(); }
        constexpr val_t y_min() const { return y_.mini(); }
        constexpr val_t y_max() const { return y_.maxi(); }
        constexpr val_t z_min() const { return z_.mini(); }
        constexpr val_t z_max() const { return z_.maxi(); }

        constexpr val_t x_mid() const { return x_.mid(); }
        constexpr val_t y_mid() const { return y_.mid(); }
        constexpr val_t z_mid() const { return z_.mid(); }
        constexpr val_t x_len() const { return x_.len(); }
        constexpr val_t y_len() const { return y_.len(); }
        constexpr val_t z_len() const { return z_.len(); }

        constexpr Vec3 mini() const {
            return Vec3{ this->x_min(), this->y_min(), this->z_min() };
        }
        constexpr Vec3 maxi() const {
            return Vec3{ this->x_max(), this->y_max(), this->z_max() };
        }
        constexpr Vec3 center() const {
            return Vec3{ this->x_mid(), this->y_mid(), this->z_mid() };
        }

        constexpr val_t volume() const {
            return this->x_len() * this->y_len() * this->z_len();
        }

        constexpr bool are_similar(const Aabb3DBase& rhs, val_t epsilon) const {
            return x_.are_similar(rhs.x_, epsilon) &&
                   y_.are_similar(rhs.y_, epsilon) &&
                   z_.are_similar(rhs.z_, epsilon);
        }

        constexpr bool is_inside_op(val_t x, val_t y, val_t z) const {
            return x_.is_inside_op(x) && y_.is_inside_op(y) &&
                   z_.is_inside_op(z);
        }
        constexpr bool is_inside_op(const Vec3& point) const {
            return this->is_inside_op(point.x(), point.y(), point.z());
        }
        constexpr bool is_inside_cl(val_t x, val_t y, val_t z) const {
            return x_.is_inside_cl(x) && y_.is_inside_cl(y) &&
                   z_.is_inside_cl(z);
        }
        constexpr bool is_inside_cl(const Vec3& point) const {
            return this->is_inside_cl(point.x(), point.y(), point.z());
        }

        constexpr bool is_intersecting_op(const Aabb3DBase& rhs) const {
            return x_.is_intersecting_op(rhs.x_) &&
                   y_.is_intersecting_op(rhs.y_) &&
                   z_.is_intersecting_op(rhs.z_);
        }
        constexpr bool is_intersecting_cl(const Aabb3DBase& rhs) const {
            return x_.is_intersecting_cl(rhs.x_) &&
                   y_.is_intersecting_cl(rhs.y_) &&
                   z_.is_intersecting_cl(rhs.z_);
        }

        constexpr bool make_intersection(
            const Aabb3DBase& rhs, Aabb3DBase& out
        ) const {
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
        constexpr OptSelf make_intersection(const Aabb3DBase& rhs) const {
            Aabb3DBase output;
            if (this->make_intersection(rhs, output))
                return output;
            else
                return sung::nullopt;
        }

        constexpr void set(val_t x, val_t y, val_t z) {
            x_.set(x);
            y_.set(y);
            z_.set(z);
        }
        constexpr void set(const Vec3& point) {
            this->set(point.x(), point.y(), point.z());
        }
        constexpr void set(
            val_t x0, val_t x1, val_t y0, val_t y1, val_t z0, val_t z1
        ) {
            x_.set(x0, x1);
            y_.set(y0, y1);
            z_.set(z0, z1);
        }
        constexpr void set(const Vec3& min, const Vec3& max) {
            this->set(min.x(), max.x(), min.y(), max.y(), min.z(), max.z());
        }

        constexpr void expand_to_span(val_t x, val_t y, val_t z) {
            x_.expand_to_span(x);
            y_.expand_to_span(y);
            z_.expand_to_span(z);
        }
        constexpr void expand_to_span(const Vec3& point) {
            this->expand_to_span(point.x(), point.y(), point.z());
        }
        constexpr Aabb3DBase get_expanded_to_span(
            val_t x, val_t y, val_t z
        ) const {
            Aabb3DBase output = *this;
            output.expand_to_span(x, y, z);
            return output;
        }
        constexpr Aabb3DBase get_expanded_to_span(const Vec3& point) const {
            return this->get_expanded_to_span(point.x(), point.y(), point.z());
        }

        constexpr void offset(val_t x, val_t y, val_t z) {
            x_.offset(x);
            y_.offset(y);
            z_.offset(z);
        }

        constexpr void offset(const Vec3& v) {
            x_.offset(v.x());
            y_.offset(v.y());
            z_.offset(v.z());
        }

    protected:
        TAabb1D x_, y_, z_;
    };

}}  // namespace sung::internal


namespace sung {

    template <typename T>
    using Aabb1D = internal::Aabb1DBase<T>;

    template <typename T>
    using Aabb2D = internal::Aabb2DBase<Aabb1D<T>>;

    template <typename T>
    using Aabb3D = internal::Aabb3DBase<Aabb1D<T>>;


    template <typename T>
    class Aabb1DLazyInit : public internal::Aabb1DBase<T> {

    public:
        constexpr void reset() { once_ = false; }

        constexpr void set_or_expand(T val) {
            if (once_)
                this->expand_to_span(val);
            else {
                this->set(val);
                once_ = true;
            }
        }

    private:
        bool once_ = false;
    };


    template <typename T>
    class Aabb2DLazyInit : public internal::Aabb2DBase<Aabb1DLazyInit<T>> {

    public:
        constexpr void reset() {
            this->x_.reset();
            this->y_.reset();
        }

        constexpr void set_or_expand(T x, T y) {
            this->x_.set_or_expand(x);
            this->y_.set_or_expand(y);
        }
    };


    template <typename T>
    class Aabb3DLazyInit : public internal::Aabb3DBase<Aabb1DLazyInit<T>> {

    public:
        using Vec3 = typename internal::Aabb3DBase<Aabb1DLazyInit<T>>::Vec3;

        constexpr void reset() {
            this->x_.reset();
            this->y_.reset();
            this->z_.reset();
        }

        constexpr void set_or_expand(T x, T y, T z) {
            this->x_.set_or_expand(x);
            this->y_.set_or_expand(y);
            this->z_.set_or_expand(z);
        }

        constexpr void set_or_expand(const Vec3& v) {
            this->x_.set_or_expand(v.x());
            this->y_.set_or_expand(v.y());
            this->z_.set_or_expand(v.z());
        }
    };

}  // namespace sung
