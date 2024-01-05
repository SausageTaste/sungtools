#include "sung/general/time.hpp"

#include <iostream>
#include <thread>


constexpr double EPSILON = 0.01;


int main() {
    sung::TimeChecker sw;
    sw.check();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    auto elapsed = sw.elapsed();

    if (elapsed < 0.5 - EPSILON || elapsed > 0.5 + EPSILON) {
        return 1;
    }

    sw.set_min();
    if (sw.elapsed() < 0) {
        return 2;
    }

    return 0;
}
