#include <gtest/gtest.h>

#include "sung/general/time.hpp"


namespace {

    TEST(Timer, TimePoint) {
        const auto tp = sung::TimePoint::from_now();

        sung::TimePoint tp_copy;
        tp_copy = tp;
        ASSERT_EQ(tp.to_time_point(), tp_copy.to_time_point());

        sung::TimePoint tp_move;
        tp_move = std::move(tp_copy);
        EXPECT_EQ(tp.to_time_point(), tp_move.to_time_point());
    }


    TEST(Timer, TimeCheckerMin) {
        sung::TimeChecker sw;
        sw.set_min();
        EXPECT_GE(sw.elapsed(), 0) << "Elapsed time of min is less than 0";
    }


    TEST(Timer, Sleep) {
        constexpr double SLEEP_SEC = 1;

        sung::TimeChecker sw;
        sung::sleep_hybrid(SLEEP_SEC);
        EXPECT_NEAR(sw.elapsed(), SLEEP_SEC, 0.000001);

        sw.check();
        sung::sleep_loop(SLEEP_SEC);
        EXPECT_NEAR(sw.elapsed(), SLEEP_SEC, 0.00001);

        sw.check();
        sung::sleep_naive(SLEEP_SEC);
        EXPECT_NEAR(sw.elapsed(), SLEEP_SEC, 0.1);
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
