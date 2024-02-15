#include <iostream>

#include "sung/general/angle.hpp"
#include "sung/general/logic_gate.hpp"


namespace {

    int test_edge_detector() {
        sung::EdgeDetector edge;

        edge.notify_signal(true);
        if (!edge.check_rising()) {
            std::cout << 1 << std::endl;
            return 1;
        }

        edge.notify_signal(true);
        if (edge.check_any_edge()){
            std::cout << 2 << std::endl;
            return 2;
        }

        edge.notify_signal(false);
        if (!edge.check_falling()) {
            std::cout << 3 << std::endl;
            return 3;
        }

        edge.notify_signal(true);
        edge.notify_signal(false);
        if (edge.check_any_edge()) {
            std::cout << 4 << std::endl;
            return 4;
        }

        return 0;
    }

    int test_rmm_realtime() {
        constexpr double TRUE_DURATION = 0.5498;
        constexpr double TOLERANCE = 0.8465416345;
        sung::RetriggerableMonostableMultivibrator<sung::TimeChecker> rmm;
        rmm.notify_signal(true);

        sung::TimeChecker timer;
        while (rmm.poll_signal(TOLERANCE)) {
            if (timer.elapsed() < TRUE_DURATION)
                rmm.notify_signal(true);
        }
        const auto elapsed = timer.elapsed();
        const auto success = sung::are_similiar(elapsed, TRUE_DURATION + TOLERANCE, 0.001);
        std::cout << "test_rmm_realtime: " << elapsed << (success ? " = " : " != ") << TRUE_DURATION + TOLERANCE << " (" << 0.001 << ")\n";

        return success ? 0 : 5;
    }

    int test_rmm_accum() {
        constexpr double TRUE_DURATION = 2.15461;
        constexpr double TOLERANCE = 0.54564;
        constexpr double STEP = 0.01345;

        sung::RetriggerableMonostableMultivibrator<sung::TimeAccumulator> rmm;
        double accum = 0;
        while (true) {
            rmm.timer().add(STEP);
            accum += STEP;

            if (accum < TRUE_DURATION)
                rmm.notify_signal(true);

            if (!rmm.poll_signal(TOLERANCE))
                break;
        }
        const auto success = sung::are_similiar(accum, TRUE_DURATION + TOLERANCE, STEP * 1.01);
        std::cout << "test_rmm_accum: " << accum << (success ? " = " : " != ") << TRUE_DURATION + TOLERANCE << " (" << STEP * 1.01 << ")\n";

        return success ? 0 : 6;
    }

}


int main() {
    if (const auto result = ::test_edge_detector())
        return result;

    if (const auto result = ::test_rmm_realtime())
        return result;

    if (const auto result = ::test_rmm_accum())
        return result;

    return 0;
}
