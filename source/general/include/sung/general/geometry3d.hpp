#pragma once

#include <array>

#include "linalg.hpp"
#include "optional.hpp"


namespace sung {

    struct SegIntersecInfo {
        // Distance from the start of the segment to the intersection point
        double distance_ = 0;
        bool from_front_ = false;
    };


    class LineSegment3 {

    public:
        using Vec3 = TVec3<double>;

        LineSegment3() = default;
        LineSegment3(const Vec3& pos, const Vec3& dir) : pos_(pos), dir_(dir) {}

        const Vec3& pos() const { return pos_; }
        const Vec3& dir() const { return dir_; }
        Vec3 end() const { return pos_ + dir_; }

        double len_sqr() const { return dir_.len_sqr(); }
        double len() const { return dir_.len(); }

    private:
        Vec3 pos_;
        Vec3 dir_;
    };


    class Plane3 {

    public:
        using Vec3 = TVec3<double>;
        using Vec4 = TVec4<double>;

        Plane3() = default;
        Plane3(const Vec3& center, const Vec3& normal)
            : center_(center), normal_(normal.normalize()) {}

        Vec4 coeff() const {
            const auto d = -normal_.dot(center_);
            return Vec4{ normal_, d };
        }

        double calc_signed_dist(const Vec3& p) const {
            return this->coeff().dot(Vec4{ p, 1 });
        }

        sung::Optional<SegIntersecInfo> find_intersection(
            const LineSegment3& seg
        ) const {
            constexpr auto ZERO = static_cast<double>(0);

            const auto pos_dist = this->calc_signed_dist(seg.pos());
            const auto end_dist = this->calc_signed_dist(seg.end());
            if ((pos_dist * end_dist) > ZERO)
                return sung::nullopt;

            const auto pos_dist_abs = std::abs(pos_dist);
            const auto denominator = pos_dist_abs + std::abs(end_dist);
            if (ZERO == denominator)
                return SegIntersecInfo{ 0, pos_dist > end_dist };

            const auto distance = seg.len() * pos_dist_abs / denominator;
            if (std::isnan(distance))
                return sung::nullopt;

            return SegIntersecInfo{ distance, pos_dist > end_dist };
        }

    private:
        Vec3 center_;
        Vec3 normal_;
    };


    class Triangle3 {

    public:
        using Vec3 = TVec3<double>;

        constexpr Triangle3() = default;
        constexpr Triangle3(const Vec3& a, const Vec3& b, const Vec3& c)
            : a_(a), b_(b), c_(c) {}

        constexpr auto& a() const { return a_; }
        constexpr auto& b() const { return b_; }
        constexpr auto& c() const { return c_; }

        double area() const {
            return (b_ - a_).cross(c_ - a_).len() / 2;
        }

        // Counter-clockwise
        Vec3 normal() const { return (b_ - a_).cross(c_ - a_).normalize(); }

        Plane3 plane() const { return Plane3{ a_, this->normal() }; }

        double radius_circumcircle() const {
            const auto ab = b_ - a_;
            const auto ac = c_ - a_;
            const auto bc = c_ - b_;

            const auto a = ab.len();
            const auto b = ac.len();
            const auto c = bc.len();

            const auto area = this->area();
            return a * b * c / (4 * area);
        }

    private:
        Vec3 a_;
        Vec3 b_;
        Vec3 c_;
    };

}  // namespace sung
