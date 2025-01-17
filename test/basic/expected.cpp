#include <iostream>
#include <string>

#include <gtest/gtest.h>

#include "sung/basic/expected.hpp"
#include "sung/basic/os_detect.hpp"


namespace {

    sung::Expected<double, std::string> divide(double a, double b) {
        sung::Expected<double, std::string> output;
        if (b == 0.0)
            output = sung::unexpected<std::string>("Division by zero");
        else
            output = a / b;
        return output;
    }


    TEST(Expected, Basic) {
        const auto result0 = ::divide(1.0, 0.0);
        EXPECT_THROW(result0.value(), sung::bad_expected_access<std::string>);
        const auto result1 = ::divide(1.0, 2.0);
        EXPECT_DOUBLE_EQ(result1.value(), 0.5);
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
