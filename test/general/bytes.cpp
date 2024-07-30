#include "sung/general/bytes.hpp"

#include <array>
#include <bit>

#include <gtest/gtest.h>

#include "sung/general/random.hpp"


// Diff calculation test
namespace {

    TEST(Bytes, Endianness) {
        ASSERT_EQ(
            sung::is_big_endian(), std::endian::native == std::endian::big
        );
        ASSERT_EQ(
            sung::is_little_endian(), std::endian::native == std::endian::little
        );
    }


    TEST(Bytes, Assemble) {
        {
            constexpr auto bytes_int = 0x01020304;
            constexpr std::array<uint8_t, 4> bytes_arr{
                0x01, 0x02, 0x03, 0x04
            };

            const auto bytes_asm = sung::assemble_be_data<int>(bytes_arr.data()
            );
            ASSERT_EQ(bytes_int, bytes_asm);
        }

        {
            constexpr auto bytes_int = 0xA1B2C3D4;
            constexpr std::array<uint8_t, 4> bytes_arr{
                0xD4, 0xC3, 0xB2, 0xA1
            };

            const auto bytes_asm = sung::assemble_le_data<int>(bytes_arr.data()
            );
            ASSERT_EQ(bytes_int, bytes_asm);
        }
    }


    TEST(Bytes, AssembleTwoWay) {
        using T = double;
        sung::RandomRealNumGenerator<T> rng{ -10000000, 10000000 };

        for (size_t i = 0; i < 10000; ++i) {
            const auto bytes_int = rng.gen();

            {
                std::array<uint8_t, sizeof(T)> bytes_arr;
                ASSERT_TRUE(sung::decompose_to_be<T>(
                    bytes_int, bytes_arr.data(), bytes_arr.size()
                ));
                const auto bytes_asm = sung::assemble_be_data<T>(bytes_arr.data(
                ));
                ASSERT_EQ(bytes_int, bytes_asm);
            }

            {
                std::array<uint8_t, sizeof(T)> bytes_arr;
                ASSERT_TRUE(sung::decompose_to_le<T>(
                    bytes_int, bytes_arr.data(), bytes_arr.size()
                ));
                const auto bytes_asm = sung::assemble_le_data<T>(bytes_arr.data(
                ));
                ASSERT_EQ(bytes_int, bytes_asm);
            }
        }
    }


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

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
