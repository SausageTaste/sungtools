#include "sung/general/cvar.hpp"

#include <gtest/gtest.h>


namespace {

    sung::AutoCVarInt cvar_int1{ "key1", "help1", 10, nullptr, sung::gcvars() };
    sung::AutoCVarFloat cvar_float1{
        "key3", "help1", 10.0, nullptr, sung::gcvars()
    };


    TEST(CVar, CVars) {
        ASSERT_TRUE(sung::gcvars().has("key1"));
        ASSERT_EQ(cvar_int1.get(), 10);
        cvar_int1.set(20);
        ASSERT_EQ(sung::gcvars().geti("key1")->get(), 20);
        sung::gcvars().geti("key1")->set(30);
        ASSERT_EQ(sung::gcvars().getf("key1"), nullptr);
        ASSERT_EQ(sung::gcvars().getf("key2"), nullptr);
        ASSERT_FALSE(sung::gcvars().has("key2"));
        ASSERT_THROW(
            sung::gcvars().create_int("key1", "", 4, nullptr),
            std::runtime_error
        );
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
