#include "sung/basic/byte_arr.hpp"

#include <gtest/gtest.h>


namespace {

    TEST(Bytes, BytesBuilder) {
        sung::BytesBuilder bb;

        std::array<double, 5> doubles{ 1.0, 2.0, 3.0, 4.0, 5.0 };
        bb.add_val_arr(doubles);

        std::array<float, 7> floats{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f };
        bb.add_val_arr(floats);

        std::array<uint64_t, 3> uints{ 0x1234567890ABCDEF,
                                       0xFEDCBA0987654321,
                                       0x0000000000000000 };
        bb.add_val_arr(uints);

        bb.add_nt_str("Hello, World!");

        sung::BytesReader br(bb.data(), bb.size());
        for (size_t i = 0; i < doubles.size(); ++i) {
            const auto val = br.read_float64();
            ASSERT_TRUE(val.has_value());
            ASSERT_EQ(doubles[i], val.value());
        }
        for (size_t i = 0; i < floats.size(); ++i) {
            const auto val = br.read_float32();
            ASSERT_TRUE(val.has_value());
            ASSERT_EQ(floats[i], val.value());
        }
        for (size_t i = 0; i < uints.size(); ++i) {
            const auto val = br.read_uint64();
            ASSERT_TRUE(val.has_value());
            ASSERT_EQ(uints[i], val.value());
        }
        ASSERT_STREQ(br.read_nt_str().c_str(), "Hello, World!");

        return;
    }

}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
