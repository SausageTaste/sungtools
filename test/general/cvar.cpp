#include "sung/general/cvar.hpp"

#include <gtest/gtest.h>


namespace {

    sung::AutoCVarInt cvar_int1{ "key1", "help1", 10 };
    sung::AutoCVarFlt cvar_float1{ "key3", "help1", 3.14 };
    sung::AutoCVarStr cvar_str1{ "key4", "help1", "value1" };


    TEST(CVar, CVars) {
        auto& cv = sung::gcvars();

        ASSERT_EQ(cvar_int1.get(), 10);
        cvar_int1.set(20);
        ASSERT_EQ(cv.geti("key1")->get(), 20);
        cv.geti("key1")->set(30);
        ASSERT_THROW(cv.create_int("key1", "", 4), std::runtime_error);

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
        ASSERT_THROW(cv.create_str("key\"2", "", ""), std::runtime_error);
    }


    TEST(CVar, Serialize) {
        sung::gcvars().create_str("key5", "help5", "Hello\n\tworld");
        sung::gcvars().create_str(
            "\xec\x95\x88\xeb\x85\x95",
            "help5",
            "Hello\n\xec\x84\xb8\xec\x83\x81"
        );
        std::cout << sung::gcvars().serialize_str() << std::endl;
    }


    class Visitor : public sung::ICVarVisitor {

    public:
        void visit(sung::ICVarInt& cvar) override {
            std::cout << "Visited " << cvar.id() << ": " << cvar.get() << '\n';
        }

        void visit(sung::ICVarFloat& cvar) override {
            std::cout << "Visited " << cvar.id() << ": " << cvar.get() << '\n';
        }

        void visit(sung::ICVarStr& cvar) override {
            std::cout << "Visited " << cvar.id() << ": " << cvar.get() << '\n';
        }
    };

    TEST(CVar, Visitor) {
        ::Visitor visitor;
        sung::gcvars().visit(visitor);
    }

}  // namespace


int main(int argc, char** argv) {
    system("chcp 65001 > nul");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
