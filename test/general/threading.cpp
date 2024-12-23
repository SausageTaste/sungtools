#include "sung/general/threading.hpp"

#include <atomic>
#include <iostream>

#include <gtest/gtest.h>

#include "sung/general/time.hpp"


namespace {

    class Task : public sung::ITask {

    public:
        Task() = default;

        sung::TaskStatus tick() override {
            ++counter_;
            return sung::TaskStatus::running;
        }

        size_t counter() { return counter_; }

    private:
        std::atomic_size_t counter_ = 0;
    };


    TEST(Threading, Basic) {
        auto scheduler = sung::create_task_scheduler();
        ASSERT_TRUE(scheduler);

        std::vector<std::shared_ptr<Task>> tasks;
        for (int i = 0; i < 32; ++i) {
            auto task = std::make_shared<Task>();
            tasks.push_back(task);
            scheduler->add_task(task);
        }

        sung::MonotonicRealtimeTimer timer;

        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            bool all_done = true;
            for (auto& task : tasks) {
                if (task->counter() < 100000) {
                    all_done = false;
                    break;
                }
            }

            if (all_done)
                break;
        }

        const auto elapsed = timer.elapsed();
        scheduler->terminate();
        for (auto& task : tasks) {
            std::cout << task->counter() << std::endl;
        }
        std::cout << "Elapsed: " << elapsed << std::endl;

        scheduler->join();
    }

}  // namespace


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
