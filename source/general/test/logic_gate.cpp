#include "sung/general/logic_gate.hpp"

#include <iostream>


int main() {
    {
        sung::EdgeDetector edge;
        edge.set_signal(true);
        if (!edge.check_rising()) {
            std::cout << 1 << std::endl;
            return 1;
        }

        edge.set_signal(true);
        if (edge.check_any_edge()){
            std::cout << 2 << std::endl;
            return 2;
        }

        edge.set_signal(false);
        if (!edge.check_falling()) {
            std::cout << 3 << std::endl;
            return 3;
        }

        edge.set_signal(true);
        edge.set_signal(false);
        if (edge.check_any_edge()) {
            std::cout << 4 << std::endl;
            return 4;
        }
    }

    return 0;
}
