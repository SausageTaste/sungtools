#include "sung/basic/geometry3d.hpp"


// Plane3
namespace sung {

    Plane3::Plane3(const Vec3& center, const Vec3& normal)
        : center_(center), normal_(normal.normalize()) {}

    Plane3::Vec4 Plane3::coeff() const {
        const auto d = -normal_.dot(center_);
        return Vec4{ normal_, d };
    }

    double Plane3::calc_signed_dist(const Vec3& p) const {
        return this->coeff().dot(Vec4{ p, 1 });
    }

    bool Plane3::find_seg_intersec(SegIntersecInfo& out, const LineSeg3& seg)
        const {
        constexpr auto ZERO = static_cast<double>(0);

        const auto pos_dist = this->calc_signed_dist(seg.pos());
        const auto end_dist = this->calc_signed_dist(seg.end());
        if ((pos_dist * end_dist) > ZERO)
            return false;

        const auto pos_dist_abs = std::abs(pos_dist);
        const auto denominator = pos_dist_abs + std::abs(end_dist);
        if (ZERO == denominator) {
            out = SegIntersecInfo{ 0, pos_dist > end_dist };
            return true;
        }

        const auto distance = seg.len() * pos_dist_abs / denominator;
        if (std::isnan(distance))
            return false;

        out = SegIntersecInfo{ distance, pos_dist > end_dist };
        return true;
    }

}  // namespace sung


// Triangle3
namespace sung {

    void Triangle3::apply_transform(const TMat4<double>& m) {
        a_ = m * Vec4{ a_, 1 };
        b_ = m * Vec4{ b_, 1 };
        c_ = m * Vec4{ c_, 1 };
    }

    double Triangle3::area() const {
        return (b_ - a_).cross(c_ - a_).len() / 2;
    }

    Triangle3::Vec3 Triangle3::normal() const {
        return (b_ - a_).cross(c_ - a_).normalize();
    }

    Plane3 Triangle3::plane() const { return Plane3{ a_, this->normal() }; }

    bool Triangle3::radius_circumcircle(double& out) const {
        const auto ab = b_ - a_;
        const auto ac = c_ - a_;
        const auto bc = c_ - b_;

        const auto a = ab.len();
        const auto b = ac.len();
        const auto c = bc.len();

        const auto cross_area = ab.cross(ac).len_sqr();
        if (cross_area <= 1e-128)
            return false;

        out = a * b * c / (2 * std::sqrt(cross_area));
        return true;
    }

    bool Triangle3::circumcenter(Vec3& out) const {
        const auto ab = b_ - a_;
        const auto bc = c_ - b_;
        const auto ca = a_ - c_;

        const auto cross_area = ab.cross(ca).len_sqr();
        if (cross_area <= 1e-128)
            return false;

        const auto A = bc.len();
        const auto B = ca.len();
        const auto C = ab.len();
        const auto r = (A * B * C) / (2 * std::sqrt(cross_area));

        const auto half_edge_len = A * 0.5;
        const auto dist_sqr = (r + half_edge_len) * (r - half_edge_len);
        const auto dist = std::sqrt(dist_sqr);

        const auto tri_normal = ab.cross(bc);
        const auto to_center = tri_normal.cross(bc).normalize() * dist;
        out = (b_ + c_) * 0.5 + to_center;
        return true;
    }

    bool Triangle3::find_seg_intersec(
        SegIntersecInfo& out, const LineSeg3& seg, bool ignore_back
    ) const {
        const auto plane = this->plane();
        const auto plane_in = plane.find_seg_intersec(seg);
        if (!plane_in)
            return false;
        if (ignore_back && !plane_in->from_front_)
            return false;

        const auto seg_dir_n = seg.dir().normalize();
        const auto p = seg.pos() + seg_dir_n * plane_in->distance_;
        const auto& n = plane.normal();

        const auto ab = b_ - a_;
        const auto bc = c_ - b_;
        const auto ca = a_ - c_;

        const auto ap = p - a_;
        const auto bp = p - b_;
        const auto cp = p - c_;

        const auto cross0 = ab.cross(ap);
        const auto cross1 = bc.cross(bp);
        const auto cross2 = ca.cross(cp);

        const auto dot0 = cross0.dot(n);
        const auto dot1 = cross1.dot(n);
        const auto dot2 = cross2.dot(n);

        if (dot0 >= 0 && dot1 >= 0 && dot2 >= 0) {
            out = plane_in.value();
            return true;
        } else if (dot0 <= 0 && dot1 <= 0 && dot2 <= 0) {
            out = plane_in.value();
            return true;
        }

        return false;
    }

}  // namespace sung


// Sphere3
namespace sung {

    Sphere3::Sphere3() : pos_{ 0, 0, 0 }, radius_(0) {}

    Sphere3::Sphere3(double radius) : pos_{ 0, 0, 0 }, radius_(radius) {}

    Sphere3::Sphere3(double x, double y, double z, double radius)
        : pos_{ x, y, z }, radius_(radius) {}

    Sphere3::Sphere3(const Vec3& pos, double radius)
        : pos_(pos), radius_(radius) {}

    bool Sphere3::is_intersecting(const sung::LineSeg3& ray) const {
        const auto oc = ray.pos() - pos_;
        const auto a = ray.dir().dot(ray.dir());
        const auto b = 2.0 * ray.dir().dot(oc);
        const auto c = oc.len_sqr() - (radius_ * radius_);
        const auto discriminant = (b * b) - (4 * a * c);
        return discriminant > 0;
    }

    bool Sphere3::find_intersection(Vec3& out, const sung::LineSeg3& ray)
        const {
        const auto oc = ray.pos() - pos_;
        const auto a = ray.dir().dot(ray.dir());
        const auto b = 2.0 * ray.dir().dot(oc);
        const auto c = oc.len_sqr() - (radius_ * radius_);
        const auto discriminant = (b * b) - (4 * a * c);
        if (discriminant < 0)
            return false;

        const auto t1 = (-b + std::sqrt(discriminant)) / (2.0 * a);
        const auto t2 = (-b - std::sqrt(discriminant)) / (2.0 * a);
        out = ray.pos() + ray.dir() * std::min(t1, t2);
        return true;
    }

}  // namespace sung


// TriSoup3
namespace sung {

    void TriSoup3::add_vtx(const Vec3& v) {
        for (size_t i = 0; i < vtx_.size(); ++i) {
            if (v.are_similar(vtx_[i])) {
                idx_.push_back(i);
                return;
            }
        }

        idx_.push_back(vtx_.size());
        vtx_.push_back(v);
    }

    size_t TriSoup3::tri_count() const { return idx_.size() / 3; }

    bool TriSoup3::find_seg_intersec(
        SegIntersecInfo& out,
        const sung::LineSegment3& ray,
        bool ignore_back
    ) const {
        OptSegIntersec res;
        const auto tri_count = this->tri_count();
        for (size_t i = 0; i < tri_count; ++i) {
            const auto i0 = idx_[i * 3 + 0];
            const auto i1 = idx_[i * 3 + 1];
            const auto i2 = idx_[i * 3 + 2];

            const sung::Triangle3 tri{ vtx_[i0], vtx_[i1], vtx_[i2] };
            const auto in = tri.find_seg_intersec(ray, ignore_back);
            if (in) {
                if (res) {
                    if (in->distance_ < res->distance_)
                        res = in;
                } else {
                    res = in;
                }
            }
        }

        if (res) {
            out = res.value();
            return true;
        }

        return false;
    }

}  // namespace sung
