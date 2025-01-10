#pragma once

#include <atomic>
#include <memory>
#include <string>


namespace sung {

    enum class TaskStatus {
        running,
        finished,
    };


    struct ITask {
        virtual ~ITask() = default;
        virtual TaskStatus tick() = 0;
    };


    class StandardLoadTask : public ITask {

    public:
        const std::string& err_msg() const { return err_msg_; }
        bool is_done() const { return done_; }
        bool has_succeeded() const { return done_ && err_msg_.empty(); }
        bool has_failed() const { return done_ && !err_msg_.empty(); }

    protected:
        TaskStatus success() {
            err_msg_.clear();
            done_ = true;
            return TaskStatus::finished;
        }

        TaskStatus fail(const char* err_msg) {
            err_msg_ = err_msg;
            done_ = true;
            return TaskStatus::finished;
        }

    private:
        std::string err_msg_;
        std::atomic_bool done_{ false };
    };


    struct ITaskScheduler {
        virtual ~ITaskScheduler() = default;
        virtual void tick() = 0;
        virtual void terminate() = 0;
        virtual void join() = 0;
        virtual void add_task(std::shared_ptr<ITask> task) = 0;
    };


    using HTaskSche = std::shared_ptr<ITaskScheduler>;
    HTaskSche create_task_scheduler();

}  // namespace sung
