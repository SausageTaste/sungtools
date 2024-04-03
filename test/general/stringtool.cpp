#include <gtest/gtest.h>

#include "sung/general/random.hpp"
#include "sung/general/stringtool.hpp"


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
            "Hello, World!:@#&(*@$(@))",
            gen_random_str(),
        };

        for (auto& x : TEST_CASES)
            std::cout << x << "   ->   " << sung::slugify(x) << '\n';
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
