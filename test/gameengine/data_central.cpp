#include "sung/gameengine/data_central.hpp"

#include <gtest/gtest.h>


namespace {

    TEST(AABB, WithDistance) {
        auto task_sche = sung::create_task_scheduler();
        auto datacen = sung::create_data_central(task_sche);
        ASSERT_NE(datacen, nullptr);
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
