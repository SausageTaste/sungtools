#include <cassert>

#include "sung/general/angle.hpp"
#include "sung/general/linalg.hpp"


namespace {

    void matrix_transform_test() {
        using Mat = sung::TMat4<float>;

        constexpr sung::TVec4<float> v{1, 0, 0, 0};

        const auto result0 = sung::TMat4<float>::rotate_axis(sung::TVec3<float>{0, 0, 1}, sung::to_radians(90.f)) * v;
        assert(result0.is_similar(sung::TVec4<float>{0, 1, 0, 0}, 0.001));

        const auto result1 = sung::TMat4<float>::rotate_axis(sung::TVec3<float>{0, 1, 0}, sung::to_radians(90.f)) * v;
        assert(result1.is_similar(sung::TVec4<float>{0, 1, 0, 0}, 0.001));

        return;
    }

}


int main() {
    constexpr sung::TVec3<float> a{1, 2, 3};
    constexpr sung::TVec3<float> b{4, 5, 6};
    constexpr auto c = a + b;

    constexpr sung::TVec4<float> d{1, 2, 3, 4};
    constexpr auto d3 = sung::TVec3<double>{d};
    constexpr auto d4 = sung::TVec4<float>{d3, 1};
    constexpr auto d5 = sung::TVec4<double>{d4};

    constexpr auto m = sung::TMat4<float>::translate(2, 3, 4);
    constexpr auto m00 = m.at(0, 3);
    constexpr auto translated = m * d4;

    ::matrix_transform_test();

    return 0;
}
