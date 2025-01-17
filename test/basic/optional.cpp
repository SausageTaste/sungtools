#include <string>

#include <gtest/gtest.h>

#include "sung/basic/optional.hpp"


namespace {

    TEST(Optional, Basic) {
        sung::Optional<std::string> opt_int;
        EXPECT_FALSE(opt_int.has_value());
        EXPECT_THROW(opt_int.value().clear(), sung::bad_optional_access);

        opt_int = "Hello, World!";
        EXPECT_TRUE(opt_int.has_value());
        EXPECT_STREQ(opt_int->c_str(), "Hello, World!");

        opt_int = sung::nullopt;
        EXPECT_FALSE(opt_int.has_value());
        EXPECT_THROW(opt_int.value().clear(), sung::bad_optional_access);

        opt_int = sung::make_optional("Hello, World!");
        EXPECT_TRUE(opt_int.has_value());
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
