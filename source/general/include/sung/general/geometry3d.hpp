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
            if (isnan(distance)) {
                throw std::exception{ "Plane::find_intersection" };
            }
            return SegIntersecInfo{ distance, pos_dist > end_dist };
        }

    private:
        Vec3 center_;
        Vec3 normal_;
    };

}  // namespace sung
