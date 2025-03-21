#include "sung/basic/time.hpp"

#include <gtest/gtest.h>

#include "sung/basic/random.hpp"
#include "sung/basic/stringtool.hpp"


namespace {

    TEST(Time, UnixTime) {
        const auto time = sung::get_time_unix();
        ASSERT_NE(time, 0);
        std::cout << std::fixed << time << std::endl;

        {
            const auto t = sung::backend::get_time_unix_time_t();
            ASSERT_NE(t, 0);
            ASSERT_NEAR(t, time, 1);
            std::cout << t << std::endl;
        }

        {
            const auto t = sung::backend::get_time_unix_chrono();
            ASSERT_NE(t, 0);
            ASSERT_NEAR(t, time, 1);
            std::cout << t << std::endl;
        }
    }


    TEST(Time, IsoTimeStr) {
        std::cout << sung::get_time_iso_utc(false) << std::endl;
        std::cout << sung::get_time_iso_utc(true) << std::endl;
    }


    TEST(Time, IsoLocalTimeStr) {
        std::cout << sung::get_time_iso_local(false) << " ("
                  << sung::get_time_iso_local_slug(false) << ")" << std::endl;
        std::cout << sung::get_time_iso_local(true) << " ("
                  << sung::get_time_iso_local_slug(true) << ")" << std::endl;
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
        cl.clock().add(value1);
        EXPECT_DOUBLE_EQ(cl.elapsed(), value1);

        const auto value2 = rng.gen();
        cl.clock().add(value2);
        EXPECT_DOUBLE_EQ(cl.elapsed(), value1 + value2);

        const auto value3 = rng.gen();
        cl.clock().set(value3);
        EXPECT_DOUBLE_EQ(cl.elapsed(), value3);

        cl.clock().set_min();
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
