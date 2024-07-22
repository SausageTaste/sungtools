#include <iostream>

#include <gtest/gtest.h>

#include "sung/general/logic_gate.hpp"
#include "sung/general/mamath.hpp"


namespace {

    TEST(LogicGate, EdgeDetector) {
        sung::EdgeDetector edge;
        edge.notify_signal(true);
        EXPECT_TRUE(edge.check_rising());

        edge.notify_signal(true);
        EXPECT_FALSE(edge.check_rising());

        edge.notify_signal(false);
        EXPECT_TRUE(edge.check_falling());

        edge.notify_signal(true);
        edge.notify_signal(false);
        EXPECT_FALSE(edge.check_any_edge());
    }


    TEST(LogicGate, Rmm) {
        constexpr double TRUE_DURATION = 0.5498;
        constexpr double TOLERANCE = 0.8465416345;

        sung::RetriggerableMMV<sung::MonotonicRealtimeTimer> rmm;
        rmm.notify_signal(true);

        sung::MonotonicRealtimeTimer timer;
        while (rmm.poll_signal(TOLERANCE)) {
            if (timer.elapsed() < TRUE_DURATION)
                rmm.notify_signal(true);
        }

        EXPECT_NEAR(timer.elapsed(), TRUE_DURATION + TOLERANCE, 1e-6);
    }


    TEST(LogicGate, RmmAcum) {
        constexpr double TRUE_DURATION = 2.15461;
        constexpr double TOLERANCE = 0.54564;
        constexpr double STEP = 0.01345;

        sung::RetriggerableMMV<sung::ManualNumericTimer> rmm;
        double accum = 0;
        while (true) {
            rmm.timer().clock_.add(STEP);
            accum += STEP;

            if (accum < TRUE_DURATION)
                rmm.notify_signal(true);

            if (!rmm.poll_signal(TOLERANCE))
                break;
        }

        EXPECT_NEAR(accum, TRUE_DURATION + TOLERANCE, STEP * 1.01);
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
