#include "sung/general/cvar.hpp"

#include <gtest/gtest.h>


namespace {

    TEST(CVar, CVars) {
        ASSERT_FALSE(sung::gcvars().has("key1"));
        ASSERT_FALSE(sung::gcvars().has("key2"));
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
