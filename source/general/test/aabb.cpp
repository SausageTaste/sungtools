#include "sung/general/aabb.hpp"


int main() {
    constexpr sung::AABB1<double> aabb{ -1, 2 };

    static_assert(aabb.minimum() == -1, "AABB1::minimum failed");
    static_assert(aabb.maximum() == 2, "AABB1::maximum failed");
    static_assert(aabb.length() == 3, "AABB1::length failed");

    static_assert(!aabb.is_inside(2), "AABB1::is_inside failed");
    static_assert(aabb.is_contacting(2), "AABB1::is_contacting failed");

    return 0;
}
