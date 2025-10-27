#include "sung/basic/densify.hpp"

#include <array>
#include <fstream>

#include <gtest/gtest.h>


namespace {

    const char* const TEST_FILE_PATH = "test_densify.aadd";


    TEST(Densify, Produce) {
        const std::array<float, 32 * 32> data;
        const std::string description = "Test AADD File";

        std::array<sung::AaddHeader::Dimension, 2> dimensions;
        dimensions[0].init(0, 1, 32, "X Axis");
        dimensions[1].init(0, 1, 32, "Y Axis");

        sung::AaddHeader header;
        header.init(
            dimensions.size(),
            description.size(),
            data.size(),
            sung::AaddHeader::DataType::float32,
            sung::AaddHeader::CompMethod::none
        );

        std::ofstream file{ TEST_FILE_PATH, std::ios::binary };
        ASSERT_TRUE(file.is_open());

        file.write(
            reinterpret_cast<const char*>(&header), sizeof(sung::AaddHeader)
        );
        file.write(
            reinterpret_cast<const char*>(dimensions.data()),
            sizeof(sung::AaddHeader::Dimension) * dimensions.size()
        );
        file.write(description.data(), description.size());
        file.write(
            reinterpret_cast<const char*>(data.data()),
            sizeof(float) * data.size()
        );
    }


    TEST(Densify, Consume) {
        std::ifstream file(TEST_FILE_PATH, std::ios::binary);
        ASSERT_TRUE(file.is_open());

        // read file to vector
        file.seekg(0, std::ios::end);
        const auto file_size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::vector<char> buffer(static_cast<size_t>(file_size));
        file.read(buffer.data(), file_size);

        const auto& header = reinterpret_cast<sung::AaddHeader*>(buffer.data());
        ASSERT_TRUE(header->is_magic_valid());

        auto dim_count = static_cast<size_t>(header->dim_count());
        auto dimensions = reinterpret_cast<sung::AaddHeader::Dimension*>(
            buffer.data() + sizeof(sung::AaddHeader)
        );
        for (size_t i = 0; i < dim_count; ++i) {
            const auto& dim = dimensions[i];
            std::cout << "Dimension " << i << ": "
                      << "min: " << dim.mini() << ", "
                      << "max: " << dim.maxi() << ", "
                      << "count: " << dim.count() << ", "
                      << "name: " << dim.name() << std::endl;
        }

        auto desc_len = static_cast<size_t>(header->desc_len());
        auto desc = reinterpret_cast<const char*>(
            buffer.data() + sizeof(sung::AaddHeader) +
            dim_count * sizeof(sung::AaddHeader::Dimension)
        );
        const std::string description(desc, desc_len);
        std::cout << "Description: '" << description << "'" << std::endl;
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
