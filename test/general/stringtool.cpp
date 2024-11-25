#include <gtest/gtest.h>

#include "sung/general/random.hpp"
#include "sung/general/stringtool.hpp"
#include "sung/general/time.hpp"


namespace {

    std::string gen_random_str() {
        sung::RandomIntegerGenerator<int> gen(1, 255);
        std::string str;
        for (int i = 0; i < 100; ++i) {
            str.push_back(static_cast<char>(gen.gen()));
        }
        return str;
    }


    TEST(StringTool, Slugify) {
        const std::vector<std::string> TEST_CASES{
            " \t  Hello, World!:@#&(*@$(@)) \t  ",
            gen_random_str(),
            sung::get_time_iso_utc(),
        };

        for (auto& x : TEST_CASES)
            std::cout << '"' << x << "\" -> \"" << sung::slugify(x, "-")
                      << "\"\n";
    }


    TEST(StringTool, Serialization) {
        const std::vector<std::string> TEST_CASES{
            "Hello, World!",
            "Hello\nWorld!",
            "Hello\rWorld!",
            "Hello\tWorld!",
            "Hello\\World!",
            "Hello\"World!",
            "Hello\nWorld!\n",
            "Hello\nWorld!\n\n",
            "Hello\nWorld!\n\n\n",
            "Hello\nWorld!\n\n\n\n",
            "Hello\nWorld!\n\n\n\n\n",
        };

        for (auto& x : TEST_CASES) {
            const auto y = sung::serialize_str(x);
            const auto z = sung::deserialize_str(y);
            ASSERT_STREQ(x.c_str(), z.c_str());
            std::cout << y << std::endl;
        }
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
