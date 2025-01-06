#include "sung/gameengine/mesh_builder.hpp"

#include <sung/general/angle.hpp>


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

}  // namespace sung


namespace sung {

    void UvSphereBuilder::build(MeshData& mesh_data) {
        using Angle = sung::TAngle<double>;
        const auto lat_min = Angle::from_deg(-89.99).normalize_neg();
        const auto lat_max = Angle::from_deg(89.99).normalize_neg();
        const auto lon_min = Angle::from_deg(-179.99).normalize_neg();
        const auto lon_max = Angle::from_deg(179.99).normalize_neg();

        const auto lon_step = (lon_max.rad() - lon_min.rad()) / slices_;
        const auto lat_step = (lat_max.rad() - lat_min.rad()) / stacks_;
        const auto len_inv = 1.0 / radius_;

        std::vector<std::vector<MeshData::Vertex>> vertex_lists;
        for (size_t i_stack = 0; i_stack <= stacks_; ++i_stack) {
            const auto lat_angle = lat_max.rad() - lat_step * i_stack;
            const auto xy = radius_ * std::cos(lat_angle);
            const auto z = radius_ * std::sin(lat_angle);

            vertex_lists.emplace_back();
            auto& vertices = vertex_lists.back();
            for (size_t i_slice = 0; i_slice <= slices_; ++i_slice) {
                vertices.emplace_back();
                auto& v = vertices.back();

                const auto lon_angle = lon_min.rad() + lon_step * i_slice;
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
