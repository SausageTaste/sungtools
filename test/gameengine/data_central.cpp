#include "sung/gameengine/data_central.hpp"

#include <gtest/gtest.h>


namespace {

    class MockDataAdapter : public sung::IDataAdapter {

    public:
        void load() override { ++load_count_; }

        void finalize() override {}

        size_t load_count_ = 0;
    };


    TEST(DataCentral, Test) {
        auto task_sche = sung::create_task_scheduler();
        auto datacen = sung::create_data_central(task_sche);
        ASSERT_NE(datacen, nullptr);

        auto reader = datacen->add(std::make_unique<MockDataAdapter>());
        auto writer = datacen->get_writer_of(reader);
        ASSERT_NE(writer, std::nullopt);

        {
            auto r0 = reader.get();
            ASSERT_NE(r0, std::nullopt);
            auto r1 = reader.get();
            ASSERT_NE(r1, std::nullopt);
            auto w0 = writer->get();
            ASSERT_EQ(w0, std::nullopt);
            auto r2 = reader.get();
            ASSERT_NE(r2, std::nullopt);
        }

        {
            auto w0 = writer->get();
            ASSERT_NE(w0, std::nullopt);
            auto r0 = reader.get();
            ASSERT_EQ(r0, std::nullopt);

            w0->second->load();
        }

        {
            auto r0 = reader.get();
            ASSERT_NE(r0, std::nullopt);

            auto adaptor = dynamic_cast<const MockDataAdapter*>(r0->second);
            ASSERT_NE(adaptor, nullptr);
            ASSERT_EQ(adaptor->load_count_, 1);
        }
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
