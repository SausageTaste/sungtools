#pragma once

#include <array>

#include "linalg.hpp"


namespace sung {

    template <typename T>
    class Triangle2 {

    public:
        using Vec2 = TVec2<T>;

        constexpr Triangle2() = default;
        constexpr Triangle2(const Vec2& a, const Vec2& b, const Vec2& c)
            : a_(a), b_(b), c_(c) {}

        constexpr auto& a() const { return a_; }
        constexpr auto& b() const { return b_; }
        constexpr auto& c() const { return c_; }

        // Closed interval
        constexpr bool is_inside_cl(const Vec2& pt) const {
            const auto d1 = this->sign(pt, a_, b_);
            const auto d2 = this->sign(pt, b_, c_);
            const auto d3 = this->sign(pt, c_, a_);

            const auto has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
            const auto has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

            return !(has_neg && has_pos);
        }

    private:
        static T sign(const Vec2& p1, const Vec2& p2, const Vec2& p3) {
            return (p1.x() - p3.x()) * (p2.y() - p3.y()) -
                   (p2.x() - p3.x()) * (p1.y() - p3.y());
        }

        Vec2 a_;
        Vec2 b_;
        Vec2 c_;
    };

}  // namespace sung
