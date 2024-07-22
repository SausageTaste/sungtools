#include "sung/general/time.hpp"

#include <gtest/gtest.h>

#include "sung/general/random.hpp"


namespace {

    TEST(Time, IsoTimeStr) {
        const auto str = sung::get_cur_time_iso_utc_strftime();
        ASSERT_FALSE(str.empty());
        std::cout << str << std::endl;

        const auto str2 = sung::get_cur_time_iso_utc();
        ASSERT_FALSE(str2.empty());
        std::cout << str2 << std::endl;
    }


    TEST(Time, MonotonicRealtimeTimerMin) {
        sung::MonotonicRealtimeTimer sw;
        sw.set_min();
        EXPECT_GE(sw.elapsed(), 0) << "Elapsed time of min is less than 0";
    }


    TEST(Time, ManualNumericTimer) {
        sung::RandomRealNumGenerator<double> rng(0, 10);
        sung::ManualNumericTimer cl;
        EXPECT_DOUBLE_EQ(cl.elapsed(), 0);

        const auto value1 = rng.gen();
        cl.clock_.add(value1);
        EXPECT_DOUBLE_EQ(cl.elapsed(), value1);

        const auto value2 = rng.gen();
        cl.clock_.add(value2);
        EXPECT_DOUBLE_EQ(cl.elapsed(), value1 + value2);

        const auto value3 = rng.gen();
        cl.clock_.set(value3);
        EXPECT_DOUBLE_EQ(cl.elapsed(), value3);

        cl.clock_.set_min();
        EXPECT_EQ(cl.elapsed(), 0);
    }


    TEST(Time, Sleep) {
        constexpr double SLEEP_SEC = 1.0 / 3.0;

        sung::MonotonicRealtimeTimer sw;
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
