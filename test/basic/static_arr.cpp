#include "sung/basic/static_arr.hpp"

#include <memory>

#include <gtest/gtest.h>


namespace {

    struct NonTrivial {
        explicit NonTrivial(int value, int& counter) : value_(value), counter_(counter) {
            ++counter;
        }

        ~NonTrivial() {
            value_ = 0;
            --counter_;
        }

        int value_ = 0;
        int& counter_;
    };


    TEST(StaticArray, Basic) {
        int counter = 0;
        sung::StaticVector<std::unique_ptr<::NonTrivial>, 5> arr;
        EXPECT_THROW(arr.front(), std::out_of_range);
        EXPECT_THROW(arr.back(), std::out_of_range);
        EXPECT_EQ(arr.front_op(), nullptr);
        EXPECT_EQ(arr.back_op(), nullptr);

        EXPECT_TRUE(arr.push_back(std::make_unique<NonTrivial>(1, counter)));
        EXPECT_TRUE(arr.push_back(std::make_unique<NonTrivial>(2, counter)));
        EXPECT_EQ(arr.size(), 2);
        EXPECT_EQ(arr[0]->value_, 1);
        EXPECT_EQ(arr[1]->value_, 2);
        EXPECT_EQ(counter, 2);

        EXPECT_TRUE(arr.push_back(std::make_unique<NonTrivial>(3, counter)));
        EXPECT_EQ(arr.front_op()->get()->value_, 1);
        EXPECT_EQ(arr.back()->value_, 3);
        EXPECT_NO_THROW(arr.at(0));
        EXPECT_NO_THROW(arr.at(1));
        EXPECT_NO_THROW(arr.at(2));
        EXPECT_THROW(arr.at(3), std::out_of_range);
        EXPECT_THROW(arr.at(4), std::out_of_range);
        EXPECT_EQ(counter, 3);

        EXPECT_TRUE(arr.pop_back());
        EXPECT_EQ(arr.front()->value_, 1);
        EXPECT_EQ(arr.back_op()->get()->value_, 2);
        EXPECT_EQ(arr.at_op(0)->get()->value_, 1);
        EXPECT_EQ(arr.at_op(1)->get()->value_, 2);
        EXPECT_EQ(arr.at_op(2), nullptr);
        EXPECT_EQ(arr.at_op(3), nullptr);
        EXPECT_EQ(counter, 2);

        EXPECT_TRUE(arr.push_back(std::make_unique<NonTrivial>(10, counter)));
        EXPECT_TRUE(arr.push_back(std::make_unique<NonTrivial>(11, counter)));
        EXPECT_TRUE(arr.push_back(std::make_unique<NonTrivial>(12, counter)));
        EXPECT_FALSE(arr.push_back(std::make_unique<NonTrivial>(13, counter)));
        EXPECT_FALSE(arr.push_back(std::make_unique<NonTrivial>(14, counter)));
        EXPECT_FALSE(arr.push_back(std::make_unique<NonTrivial>(15, counter)));
        EXPECT_FALSE(arr.push_back(std::make_unique<NonTrivial>(16, counter)));
        EXPECT_EQ(arr.size(), 5);
        EXPECT_EQ(arr.back().get()->value_, 12);

        EXPECT_EQ(counter, 5);
        arr.clear();
        EXPECT_EQ(counter, 0);
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
