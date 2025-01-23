#include "sung/basic/mesh_builder.hpp"

#include "sung/basic/angle.hpp"


namespace {

    void calc_tangents(
        sung::MeshData::Vertex& p0,
        sung::MeshData::Vertex& p1,
        sung::MeshData::Vertex& p2
    ) {
        auto edge1 = p1.pos_ - p0.pos_;
        auto edge2 = p2.pos_ - p0.pos_;
        auto d_uv1 = p1.texco0_ - p0.texco0_;
        auto d_uv2 = p2.texco0_ - p0.texco0_;
        const auto deno = d_uv1.x() * d_uv2.y() - d_uv2.x() * d_uv1.y();
        if (0 == deno)
            return;

        const auto f = 1 / deno;
        sung::MeshData::Vec3 tangent;
        tangent.x() = f * (d_uv2.y() * edge1.x() - d_uv1.y() * edge2.x());
        tangent.y() = f * (d_uv2.y() * edge1.y() - d_uv1.y() * edge2.y());
        tangent.z() = f * (d_uv2.y() * edge1.z() - d_uv1.y() * edge2.z());
        tangent = tangent.normalize();

        p0.tangent_ = tangent;
        p1.tangent_ = tangent;
        p2.tangent_ = tangent;
    }

}  // namespace


// MeshData
namespace sung {

    bool MeshData::Vertex::operator==(const Vertex& rhs) const {
        return (pos_.are_similar(rhs.pos_)) &&
               (normal_.are_similar(rhs.normal_)) &&
               (texco0_.are_similar(rhs.texco0_));
    }

    MeshData& MeshData::add_vertex(const Vertex& vertex) {
        for (size_t i = 0; i < vertices_.size(); ++i) {
            if (vertices_[i] == vertex) {
                indices_.push_back(i);
                return *this;
            }
        }

        indices_.push_back(vertices_.size());
        vertices_.push_back(vertex);
        return *this;
    }

    MeshData& MeshData::add_quad(
        const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3
    ) {
        this->add_vertex(v0)
            .add_vertex(v1)
            .add_vertex(v2)
            .add_vertex(v0)
            .add_vertex(v2)
            .add_vertex(v3);
        return *this;
    }

    MeshData& MeshData::add_aa_xy_quad(
        double x0, double x1, double y0, double y1, double z
    ) {
        const AABB1<double> x{ x0, x1 };
        const AABB1<double> y{ y0, y1 };

        Vertex v0;
        v0.pos_.x() = x.mini();
        v0.pos_.y() = y.maxi();
        v0.pos_.z() = z;
        v0.normal_ = { 0, 0, 1 };
        v0.texco0_ = { 0, 1 };

        Vertex v1;
        v1.pos_.x() = x.mini();
        v1.pos_.y() = y.mini();
        v1.pos_.z() = z;
        v1.normal_ = { 0, 0, 1 };
        v1.texco0_ = { 0, 0 };

        Vertex v2;
        v2.pos_.x() = x.maxi();
        v2.pos_.y() = y.mini();
        v2.pos_.z() = z;
        v2.normal_ = { 0, 0, 1 };
        v2.texco0_ = { 1, 0 };

        Vertex v3;
        v3.pos_.x() = x.maxi();
        v3.pos_.y() = y.maxi();
        v3.pos_.z() = z;
        v3.normal_ = { 0, 0, 1 };
        v3.texco0_ = { 1, 1 };

        this->add_quad(v0, v1, v2, v3);
        return *this;
    }

    MeshData& MeshData::add_aa_xz_quad(
        double x0, double x1, double z0, double z1, double y
    ) {
        const AABB1<double> x{ x0, x1 };
        const AABB1<double> z{ z0, z1 };

        Vertex v0;
        v0.pos_.x() = x.mini();
        v0.pos_.y() = y;
        v0.pos_.z() = z.mini();
        v0.normal_ = { 0, 1, 0 };
        v0.texco0_ = { 0, 1 };

        Vertex v1;
        v1.pos_.x() = x.mini();
        v1.pos_.y() = y;
        v1.pos_.z() = z.maxi();
        v0.normal_ = { 0, 1, 0 };
        v1.texco0_ = { 0, 0 };

        Vertex v2;
        v2.pos_.x() = x.maxi();
        v2.pos_.y() = y;
        v2.pos_.z() = z.maxi();
        v0.normal_ = { 0, 1, 0 };
        v2.texco0_ = { 1, 0 };

        Vertex v3;
        v3.pos_.x() = x.maxi();
        v3.pos_.y() = y;
        v3.pos_.z() = z.mini();
        v3.normal_ = { 0, 1, 0 };
        v3.texco0_ = { 1, 1 };

        this->add_quad(v0, v1, v2, v3);
        return *this;
    }

    void MeshData::build_tangents() {
        const auto tri_count = indices_.size() / 3;
        for (size_t i = 0; i < tri_count; ++i) {
            const auto i0 = indices_.at(i * 3 + 0);
            const auto i1 = indices_.at(i * 3 + 1);
            const auto i2 = indices_.at(i * 3 + 2);

            auto& v0 = vertices_.at(i0);
            auto& v1 = vertices_.at(i1);
            auto& v2 = vertices_.at(i2);

            ::calc_tangents(v0, v1, v2);
        }
    }

}  // namespace sung


namespace sung {

    void UvSphereBuilder::build(MeshData& mesh_data) const {
        using Angle = sung::TAngle<double>;

        const auto lon_step = (lon_max_.rad() - lon_min_.rad()) / slices_;
        const auto lat_step = (lat_max_.rad() - lat_min_.rad()) / stacks_;
        const auto len_inv = 1.0 / radius_;

        std::vector<std::vector<MeshData::Vertex>> vertex_lists;
        for (size_t i_stack = 0; i_stack <= stacks_; ++i_stack) {
            const auto lat_angle = lat_max_.rad() - lat_step * i_stack;
            const auto xy = radius_ * std::cos(lat_angle);
            const auto z = radius_ * std::sin(lat_angle);

            vertex_lists.emplace_back();
            auto& vertices = vertex_lists.back();
            for (size_t i_slice = 0; i_slice <= slices_; ++i_slice) {
                vertices.emplace_back();
                auto& v = vertices.back();

                const auto lon_angle = lon_min_.rad() + lon_step * i_slice;
                v.pos_.x() = xy * std::cos(lon_angle);
                v.pos_.y() = xy * std::sin(lon_angle);
                v.pos_.z() = z;
                v.normal_ = v.pos_ * len_inv;
                v.texco0_.x() = 1.0 - static_cast<double>(i_slice) / slices_;
                v.texco0_.y() = static_cast<double>(i_stack) / stacks_;
            }
        }

        const auto stack_count = vertex_lists.size() - 1;
        const auto slice_count = vertex_lists[0].size() - 1;
        for (int i = 0; i < stack_count; ++i) {
            const auto i_next = i + 1;

            for (int j = 0; j < slice_count; ++j) {
                const auto j_next = j + 1;

                mesh_data.add_quad(
                    vertex_lists[i][j],
                    vertex_lists[i_next][j],
                    vertex_lists[i_next][j_next],
                    vertex_lists[i][j_next]
                );
            }
        }

        return;
    }

}  // namespace sung
