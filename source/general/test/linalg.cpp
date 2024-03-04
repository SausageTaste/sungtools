#include <cassert>

#include "sung/general/angle.hpp"
#include "sung/general/linalg.hpp"


namespace {

    void matrix_transform_test() {
        using Vec3 = sung::TVec4<float>;
        using Vec4 = sung::TVec4<float>;
        using Mat = sung::TMat4<float>;

        constexpr sung::TVec3<float> v{1, 0, 0};
        constexpr auto angle = sung::to_radians(90.f);

        const auto result0 = Mat::rotate_axis({0, 0, 1}, angle) * Vec4(v, 0);
        assert(result0.are_similar({0, 1, 0, 0}, 0.001f));

        const auto result1 = Mat::rotate_axis({0, 1, 0}, angle) * Vec4(v, 0);
        assert(result1.are_similar({0, 0, -1, 0}, 0.001f));

        const auto result2 = Mat::rotate_axis({0, 1, 0}, angle) * Mat::rotate_axis({1, 0, 0}, -angle) * Vec4(v, 1);
        assert(result1.are_similar({0, 0, -1, 0}, 0.001f));

        constexpr auto result3 = Mat::translate(1, 2, 3) * Mat::translate(1, -2, 3) * Vec4(v, 1);
        static_assert(result3.are_similar({3, 0, 6, 1}, 0.001f), "");

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
