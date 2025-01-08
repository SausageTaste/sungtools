#pragma once

#include <array>
#include <vector>

#include "linalg.hpp"
#include "optional.hpp"


namespace sung {

    struct SegIntersecInfo {
        // Distance from the start of the segment to the intersection point
        double distance_ = 0;
        bool from_front_ = false;
    };
    using OptSegIntersec = sung::Optional<SegIntersecInfo>;


    class LineSegment3 {

    public:
        using Vec3 = TVec3<double>;
        using Vec4 = TVec4<double>;

        LineSegment3() = default;
        LineSegment3(const Vec3& pos, const Vec3& dir) : pos_(pos), dir_(dir) {}

        void apply_transform(const TMat4<double>& m) {
            pos_ = m * Vec4{ pos_, 1 };
            dir_ = m * Vec4{ dir_, 0 };
        }

        const Vec3& pos() const { return pos_; }
        const Vec3& dir() const { return dir_; }
        Vec3 end() const { return pos_ + dir_; }

        double len_sqr() const { return dir_.len_sqr(); }
        double len() const { return dir_.len(); }

    private:
        Vec3 pos_;
        Vec3 dir_;
    };
    using LineSeg3 = LineSegment3;


    class Plane3 {

    public:
        using Vec3 = TVec3<double>;
        using Vec4 = TVec4<double>;

        Plane3() = default;
        Plane3(const Vec3& center, const Vec3& normal);

        const Vec3& center() const { return center_; }
        const Vec3& normal() const { return normal_; }

        Vec4 coeff() const;
        double calc_signed_dist(const Vec3& p) const;

        bool find_seg_intersec(SegIntersecInfo& out, const LineSeg3& seg) const;
        OptSegIntersec find_seg_intersec(const LineSeg3& seg) const {
            SegIntersecInfo out;
            if (this->find_seg_intersec(out, seg))
                return out;
            return sung::nullopt;
        }

    private:
        Vec3 center_;
        Vec3 normal_;
    };


    class Triangle3 {

    public:
        using Vec3 = Plane3::Vec3;
        using Vec4 = Plane3::Vec4;

        constexpr Triangle3() = default;
        constexpr Triangle3(const Vec3& a, const Vec3& b, const Vec3& c)
            : a_(a), b_(b), c_(c) {}

        constexpr auto& a() const { return a_; }
        constexpr auto& b() const { return b_; }
        constexpr auto& c() const { return c_; }

        void apply_transform(const TMat4<double>& m);

        double area() const;
        // Counter-clockwise
        Vec3 normal() const;
        Plane3 plane() const;

        sung::Optional<double> radius_circumcircle() const {
            double out;
            if (this->radius_circumcircle(out))
                return out;
            return sung::nullopt;
        }

        // https://www.desmos.com/3d/xf0tqmgkfo
        sung::Optional<Vec3> circumcenter() const {
            Vec3 out;
            if (this->circumcenter(out))
                return out;
            return sung::nullopt;
        }

        bool find_seg_intersec(
            SegIntersecInfo& out, const LineSeg3& seg, bool ignore_back
        ) const;

        OptSegIntersec find_seg_intersec(const LineSeg3& seg, bool ignore_back)
            const {
            SegIntersecInfo out;
            if (this->find_seg_intersec(out, seg, ignore_back))
                return out;
            return sung::nullopt;
        }

    private:
        bool radius_circumcircle(double& out) const;
        bool circumcenter(Vec3& out) const;

        Vec3 a_;
        Vec3 b_;
        Vec3 c_;
    };


    class Sphere3 {

    public:
        using Vec3 = sung::TVec3<double>;

        Sphere3();
        Sphere3(double radius);
        Sphere3(double x, double y, double z, double radius);
        Sphere3(const Vec3& pos, double radius);

        bool is_intersecting(const LineSeg3& ray) const;
        bool find_intersection(Vec3& out, const LineSeg3& ray) const;

        sung::Optional<Vec3> find_ray_intersec(const LineSeg3& ray) const {
            Vec3 out;
            if (this->find_intersection(out, ray))
                return out;
            return sung::nullopt;
        }

        Vec3 pos_;
        double radius_;
    };


    // Triangle soup
    class TriSoup3 {

    public:
        using Vec3 = Triangle3::Vec3;
        using Vec4 = Triangle3::Vec4;

        void add_vtx(const Vec3& v);

        size_t tri_count() const;

        bool find_seg_intersec(
            SegIntersecInfo& out,
            const sung::LineSegment3& ray,
            bool ignore_back
        ) const;

        OptSegIntersec find_seg_intersec(
            const sung::LineSegment3& ray, bool ignore_back
        ) const {
            SegIntersecInfo out;
            if (this->find_seg_intersec(out, ray, ignore_back))
                return out;
            return sung::nullopt;
        }

        std::vector<Vec3> vtx_;
        std::vector<uint32_t> idx_;
    };

}  // namespace sung
