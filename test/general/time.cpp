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


    TEST(Time, CalenderTime) {
        const auto tp = sung::CalenderTime::from_now();

        sung::CalenderTime tp_copy;
        tp_copy = tp;
        ASSERT_EQ(tp.to_time_point(), tp_copy.to_time_point());

        sung::CalenderTime tp_move;
        tp_move = std::move(tp_copy);
        EXPECT_EQ(tp.to_time_point(), tp_move.to_time_point());

        std::cout << tp.make_sortable_text(false) << std::endl;
        std::cout << tp.make_sortable_text(true) << std::endl;
        std::cout << tp.make_locale_text() << std::endl;
    }


    TEST(Time, MonotonicClockMin) {
        sung::MonotonicClock sw;
        sw.set_min();
        EXPECT_GE(sw.elapsed(), 0) << "Elapsed time of min is less than 0";
    }


    TEST(Time, ManualClock) {
        sung::RandomRealNumGenerator<double> rng(0, 10);
        sung::ManualClock cl;
        EXPECT_DOUBLE_EQ(cl.elapsed(), 0);

        const auto value1 = rng.gen();
        cl.add(value1);
        EXPECT_DOUBLE_EQ(cl.elapsed(), value1);

        const auto value2 = rng.gen();
        cl.add(value2);
        EXPECT_DOUBLE_EQ(cl.elapsed(), value1 + value2);

        const auto value3 = rng.gen();
        cl.set(value3);
        EXPECT_DOUBLE_EQ(cl.elapsed(), value3);

        cl.set_min();
        EXPECT_EQ(cl.elapsed(), 0);
    }


    TEST(Time, Sleep) {
        constexpr double SLEEP_SEC = 1.0 / 3.0;

        sung::MonotonicClock sw;
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
