#include "sung/general/units.hpp"


int main() {
    constexpr sung::TDistance<double> distance = sung::TDistance<double>::from_feet(1000);
    constexpr sung::TSpeed<double> speed = sung::TSpeed<double>::from_kts(100);
    return 0;
}
