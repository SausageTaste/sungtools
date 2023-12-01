#include "sung/general/time.hpp"

#include <iostream>
#include <thread>


constexpr double EPSILON = 0.01;


int main() {
    sung::TimeChecker sw;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    const auto elapsed = sw.elapsed();

    if (elapsed < 0.5 - EPSILON || elapsed > 0.5 + EPSILON) {
        return 1;
    }

    return 0;
}
