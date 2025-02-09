#pragma once

#include <atomic>
#include <memory>
#include <string>


namespace sung {

    enum class TaskStatus {
        running,
        finished,
    };


    class ITask {

    public:
        virtual ~ITask() = default;
        virtual TaskStatus tick() = 0;

        int16_t priority() const;
        void set_priority(int16_t priority);

    private:
        std::atomic_int16_t priority_ = 0;
    };


    class IStandardLoadTask : public ITask {

    public:
        enum class Status {
            running,
            success,
            failed,
        };

        const std::string& err_msg() const;

        bool is_done() const;
        bool has_succeeded() const;
        bool has_failed() const;
        Status status() const;

    protected:
        static TaskStatus running();
        TaskStatus success();
        TaskStatus fail(const char* err_msg);
        TaskStatus fail(const std::string& err_msg);
        TaskStatus fail(std::string&& err_msg);

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
