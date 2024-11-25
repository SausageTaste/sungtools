#include "sung/general/cvar.hpp"

#include <gtest/gtest.h>


namespace {

    sung::AutoCVarInt cvar_int1{ "key1", "help1", 10, nullptr, sung::gcvars() };
    sung::AutoCVarFloat cvar_float1{
        "key3", "help1", 3.14, nullptr, sung::gcvars()
    };
    sung::AutoCVarStr cvar_str1{
        "key4", "help1", "value1", nullptr, sung::gcvars()
    };


    TEST(CVar, CVars) {
        auto& cv = sung::gcvars();

        ASSERT_EQ(cvar_int1.get(), 10);
        cvar_int1.set(20);
        ASSERT_EQ(cv.geti("key1")->get(), 20);
        cv.geti("key1")->set(30);
        ASSERT_THROW(cv.create_int("key1", "", 4, nullptr), std::runtime_error);

        ASSERT_EQ(cv.getf("key1"), nullptr);
        ASSERT_EQ(cv.getf("key2"), nullptr);
        ASSERT_EQ(cv.getf("key3")->get(), 3.14);
        cvar_float1.set(2.71);
        ASSERT_EQ(cv.getf("key3")->get(), 2.71);

        ASSERT_STREQ(cv.gets("key4")->get().c_str(), "value1");
        cvar_str1.set("value2");
        ASSERT_STREQ(cv.gets("key4")->get().c_str(), "value2");
    }


    TEST(CVar, Predicate) {
        auto& cv = sung::gcvars();

        sung::AutoCVarInt cvar_int2{
            "key2", "help2", 10, [](int64_t v) { return v > 0; }, cv
        };
        ASSERT_EQ(cvar_int2.get(), 10);
        ASSERT_FALSE(cvar_int2.set(-1));
        ASSERT_EQ(cvar_int2.get(), 10);
        ASSERT_TRUE(cvar_int2.set(20));
        ASSERT_EQ(cvar_int2.get(), 20);
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
