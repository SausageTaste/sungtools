#include "sung/basic/static_arr.hpp"

#include <memory>

#include <gtest/gtest.h>


namespace {

    struct NonTrivial {
        NonTrivial(int v, int& c) : value_(v), counter_(&c) { ++(*counter_); }
        ~NonTrivial() { --(*counter_); }

        int value_;
        int* counter_;
    };

    TEST(StaticVectorTest, PushPopAccess) {
        int counter = 0;
        sung::StaticVector<std::unique_ptr<::NonTrivial>, 5> arr;

        // Initial state
        EXPECT_EQ(arr.size(), 0);
        EXPECT_THROW(arr.front(), std::out_of_range);
        EXPECT_THROW(arr.back(), std::out_of_range);
        EXPECT_EQ(arr.front_op(), nullptr);
        EXPECT_EQ(arr.back_op(), nullptr);

        // Push 3 elements
        EXPECT_TRUE(arr.push_back(std::make_unique<NonTrivial>(10, counter)));
        EXPECT_TRUE(arr.push_back(std::make_unique<NonTrivial>(20, counter)));
        EXPECT_TRUE(arr.push_back(std::make_unique<NonTrivial>(30, counter)));
        EXPECT_EQ(arr.size(), 3);
        EXPECT_EQ(counter, 3);

        // Check access
        EXPECT_EQ(arr.front()->value_, 10);
        EXPECT_EQ(arr.back()->value_, 30);
        EXPECT_NO_THROW(arr.at(0));
        EXPECT_NO_THROW(arr.at(2));
        EXPECT_THROW(arr.at(3), std::out_of_range);
        EXPECT_EQ(arr.at_op(2)->get()->value_, 30);
        EXPECT_EQ(arr.at_op(3), nullptr);

        // Pop one element
        EXPECT_TRUE(arr.pop_back());
        EXPECT_EQ(arr.size(), 2);
        EXPECT_EQ(arr.back()->value_, 20);
        EXPECT_EQ(counter, 2);

        // Fill to capacity
        EXPECT_TRUE(arr.push_back(std::make_unique<NonTrivial>(40, counter)));
        EXPECT_TRUE(arr.push_back(std::make_unique<NonTrivial>(50, counter)));
        EXPECT_TRUE(arr.push_back(std::make_unique<NonTrivial>(60, counter)));
        EXPECT_FALSE(arr.push_back(std::make_unique<NonTrivial>(70, counter)));
        EXPECT_EQ(arr.size(), 5);
        EXPECT_EQ(arr.back()->value_, 60);
        EXPECT_EQ(counter, 5);
    }

    TEST(StaticVectorTest, ClearAndReuse) {
        int counter = 0;
        sung::StaticVector<std::unique_ptr<NonTrivial>, 3> arr;

        arr.push_back(std::make_unique<NonTrivial>(1, counter));
        arr.push_back(std::make_unique<NonTrivial>(2, counter));
        arr.push_back(std::make_unique<NonTrivial>(3, counter));
        EXPECT_EQ(counter, 3);

        arr.clear();
        EXPECT_EQ(counter, 0);
        EXPECT_EQ(arr.size(), 0);

        // Reuse
        arr.push_back(std::make_unique<NonTrivial>(99, counter));
        EXPECT_EQ(arr.front()->value_, 99);
        EXPECT_EQ(counter, 1);
    }

    TEST(StaticVectorTest, BoundsSafety) {
        int counter = 0;
        sung::StaticVector<std::unique_ptr<NonTrivial>, 2> arr;

        EXPECT_EQ(arr.at_op(0), nullptr);
        EXPECT_THROW(arr.at(0), std::out_of_range);

        arr.push_back(std::make_unique<NonTrivial>(42, counter));
        EXPECT_NO_THROW(arr.at(0));
        EXPECT_THROW(arr.at(1), std::out_of_range);
        EXPECT_EQ(arr.at_op(1), nullptr);
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
