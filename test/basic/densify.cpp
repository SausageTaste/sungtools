#include "sung/basic/densify.hpp"

#include <fstream>

#include <gtest/gtest.h>


namespace {

    TEST(Expected, Basic) {
        const char* FILE_PATH =
            "C:\\Users\\AORUS\\AppData\\Roaming\\KorSimGL\\cache\\elev\\ETOPO_"
            "2022_v1_15s_N90E105_surface.aadd";
        std::ifstream file(FILE_PATH, std::ios::binary);
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
