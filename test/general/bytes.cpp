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

            const auto bytes_asm = sung::assemble_from_big_endian<int>(
                bytes_arr.data()
            );
            ASSERT_EQ(bytes_int, bytes_asm);
        }

        {
            constexpr auto bytes_int = 0xA1B2C3D4;
            constexpr std::array<uint8_t, 4> bytes_arr{
                0xD4, 0xC3, 0xB2, 0xA1
            };

            const auto bytes_asm = sung::assemble_from_little_endian<int>(
                bytes_arr.data()
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
                ASSERT_TRUE(sung::decompose_to_big_endian<T>(
                    bytes_int, bytes_arr.data(), bytes_arr.size()
                ));
                const auto bytes_asm = sung::assemble_from_big_endian<T>(
                    bytes_arr.data()
                );
                ASSERT_EQ(bytes_int, bytes_asm);
            }

            {
                std::array<uint8_t, sizeof(T)> bytes_arr;
                ASSERT_TRUE(sung::decompose_to_little_endian<T>(
                    bytes_int, bytes_arr.data(), bytes_arr.size()
                ));
                const auto bytes_asm = sung::assemble_from_little_endian<T>(
                    bytes_arr.data()
                );
                ASSERT_EQ(bytes_int, bytes_asm);
            }
        }
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
