#include "sung/gameengine/data_central.hpp"

#include <gtest/gtest.h>


namespace {

    class MockDataAdapter : public sung::IDataAdapter {

    public:
        void load() override { ++load_count_; }

        void finalize() override {}

        size_t load_count_ = 0;
    };


    TEST(DataCentral, Test) {}

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
