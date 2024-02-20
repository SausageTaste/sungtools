#include "sung/general/aabb.hpp"


int main() {
    constexpr sung::AABB1<double> aabb{ -1, 2 };

    static_assert(aabb.minimum() == -1, "AABB1::minimum failed");
    static_assert(aabb.maximum() == 2, "AABB1::maximum failed");
    static_assert(aabb.length() == 3, "AABB1::length failed");

    static_assert(!aabb.is_inside(2), "AABB1::is_inside failed");
    static_assert(aabb.is_contacting(2), "AABB1::is_contacting failed");

    constexpr auto aabb2 = sung::AABB1<double>(-2.3, 1.4);
    constexpr auto intersection = aabb.make_intersection(aabb2);

    constexpr auto aabb3 = sung::AABB1<double>(5.3, 2.4);
    constexpr auto intersection2 = aabb.make_intersection(aabb3);
    static_assert(intersection2 == sung::nullopt, "AABB1::make_intersection failed");

    return 0;
}
