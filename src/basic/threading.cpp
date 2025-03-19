#include "sung/basic/threading.hpp"

#include <array>
#include <deque>
#include <mutex>
#include <thread>
#include <vector>

#include "sung/basic/time.hpp"


namespace {

    class TaskList {

    public:
        struct Record {
            sung::MonotonicRealtimeTimer select_time_;
            std::shared_ptr<sung::ITask> task_;
            bool occupied_ = false;
        };

        void add_task(std::shared_ptr<sung::ITask> task) {
            std::lock_guard<std::mutex> lock(mut_);

            auto& r = this->find_empty_slot();
            r.select_time_.check();
            r.task_ = task;
            r.occupied_ = false;
        }

        void remove_task(sung::ITask& task) {
            std::lock_guard<std::mutex> lock(mut_);

            for (auto& r : tasks_) {
                if (r.task_.get() == &task) {
                    r.task_.reset();
                    r.occupied_ = false;
                }
            }
        }

        void yield(sung::ITask& task) {
            std::lock_guard<std::mutex> lock(mut_);
            for (auto& r : tasks_) {
                if (r.task_.get() == &task) {
                    r.occupied_ = false;
                    break;
                }
            }
        }

        // Might be null.
        std::shared_ptr<sung::ITask> select() {
            double max_wait = 0;
            Record* out = nullptr;
            std::lock_guard<std::mutex> lock(mut_);

            for (auto& r : tasks_) {
                if (!r.task_)
                    continue;
                if (r.occupied_)
                    continue;

                const auto elapsed = r.select_time_.elapsed();
                if (elapsed > max_wait) {
                    max_wait = elapsed;
                    out = &r;
                }
            }

            if (out) {
                out->select_time_.check();
                out->occupied_ = true;
                return out->task_;
            }

            return nullptr;
        }

    private:
        // Iterators are invalidated, references are not.
        Record& find_empty_slot() {
            for (auto& r : tasks_) {
                if (!r.task_) {
                    return r;
                }
            }

            tasks_.emplace_back();
            return tasks_.back();
        }

        std::deque<Record> tasks_;
        std::mutex mut_;
    };


    class ThreadFunc {

    public:
        void operator()() {
            while (!quit_flag_) {
                const auto task = this->select_task();
                if (!task) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    continue;
                }

                const auto status = task->tick();
                if (status == sung::TaskStatus::finished) {
                    this->remove_task(*task);
                } else {
                    this->yield_task(*task);
                }

                std::this_thread::yield();
            }
        }

        void give_tasks(::TaskList& tasks) { tasks_ = &tasks; }

        void set_terminate_flag() { quit_flag_ = true; }

    private:
        std::shared_ptr<sung::ITask> select_task() {
            if (tasks_)
                return tasks_->select();
            else
                return nullptr;
        }

        void remove_task(sung::ITask& task) {
            if (tasks_)
                tasks_->remove_task(task);
        }

        void yield_task(sung::ITask& task) {
            if (tasks_)
                tasks_->yield(task);
        }

        ::TaskList* tasks_ = nullptr;
        std::atomic_bool quit_flag_ = false;
    };


    class TaskScheduler : public sung::ITaskScheduler {

    public:
        TaskScheduler(size_t thread_count)
            : functions_(thread_count), threads_(thread_count) {
            for (int i = 0; i < thread_count; ++i) {
                functions_[i].give_tasks(tasks_);
                threads_[i] = std::thread(std::ref(functions_[i]));
            }
        }

        ~TaskScheduler() {
            this->terminate();
            this->join();
        }

        void tick() override {}

        void terminate() override {
            for (auto& f : functions_) {
                f.set_terminate_flag();
            }
        }

        void join() override {
            for (auto& t : threads_) {
                t.join();
            }
        }

        void add_task(std::shared_ptr<sung::ITask> task) override {
            tasks_.add_task(task);
        }

    private:
        std::vector<::ThreadFunc> functions_;
        std::vector<std::thread> threads_;
        ::TaskList tasks_;
    };

}  // namespace


// ITask
namespace sung {

    int16_t ITask::priority() const { return priority_.load(); }

    void ITask::set_priority(int16_t priority) { priority_.store(priority); }

}  // namespace sung


// IStandardLoadTask
namespace sung {

    const std::string& IStandardLoadTask::err_msg() const { return err_msg_; }

    bool IStandardLoadTask::is_done() const { return done_; }

    bool IStandardLoadTask::has_succeeded() const {
        return done_ && err_msg_.empty();
    }

    bool IStandardLoadTask::has_failed() const {
        return done_ && !err_msg_.empty();
    }

    IStandardLoadTask::Status IStandardLoadTask::status() const {
        if (done_) {
            return err_msg_.empty() ? Status::success : Status::failed;
        }
        return Status::running;
    }

    void IStandardLoadTask::wait_spinlock() const {
        while (!done_) {
            std::this_thread::yield();
        }
    }

    TaskStatus IStandardLoadTask::running() { return TaskStatus::running; }

    TaskStatus IStandardLoadTask::success() {
        err_msg_.clear();
        done_ = true;
        return TaskStatus::finished;
    }

    TaskStatus IStandardLoadTask::fail(const char* err_msg) {
        err_msg_ = err_msg;
        done_ = true;
        return TaskStatus::finished;
    }

    TaskStatus IStandardLoadTask::fail(const std::string& err_msg) {
        err_msg_ = err_msg;
        done_ = true;
        return TaskStatus::finished;
    }

    TaskStatus IStandardLoadTask::fail(std::string&& err_msg) {
        err_msg_ = std::move(err_msg);
        done_ = true;
        return TaskStatus::finished;
    }

}  // namespace sung


namespace sung {

    HTaskSche create_task_scheduler() {
        return create_task_scheduler(std::thread::hardware_concurrency());
    }

    HTaskSche create_task_scheduler(size_t thread_count) {
        return std::make_shared<TaskScheduler>(thread_count);
    }

}  // namespace sung
