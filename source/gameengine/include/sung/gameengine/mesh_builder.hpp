#pragma once

#include <vector>

#include "sung/general/aabb.hpp"
#include "sung/general/angle.hpp"
#include "sung/general/linalg.hpp"


namespace sung {

    class MeshData {

    public:
        using Vec3 = sung::TVec3<double>;
        using Vec2 = sung::TVec2<double>;

        struct Vertex {
            bool operator==(const Vertex& rhs) const;

            Vec3 pos_;
            Vec3 normal_;
            Vec2 texco0_;
        };

        MeshData& add_vertex(const Vertex& vertex);
        MeshData& add_quad(
            const Vertex& v0,
            const Vertex& v1,
            const Vertex& v2,
            const Vertex& v3
        );

        std::vector<Vertex> vertices_;
        std::vector<size_t> indices_;
    };


    class UvSphereBuilder {

    public:
        using Angle = sung::TAngle<double>;

        void build(MeshData& mesh_data) const;

        MeshData build() const {
            MeshData mesh_data;
            this->build(mesh_data);
            return mesh_data;
        }

        Angle lat_min_ = Angle::from_deg(-90);
        Angle lat_max_ = Angle::from_deg(90);
        Angle lon_min_ = Angle::from_deg(-180);
        Angle lon_max_ = Angle::from_deg(180);

        double radius_ = 1.0;
        size_t slices_ = 16;
        size_t stacks_ = 8;
    };

}  // namespace sung
