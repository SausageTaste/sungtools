#pragma once

#include <memory>


namespace sung {

    enum class TaskStatus {
        running,
        finished,
    };


    struct ITask {
        virtual ~ITask() = default;
        virtual TaskStatus tick() = 0;
    };


    struct ITaskScheduler {
        virtual ~ITaskScheduler() = default;
        virtual void tick() = 0;
        virtual void terminate() = 0;
        virtual void join() = 0;
        virtual void add_task(std::shared_ptr<ITask> task) = 0;
    };


    std::shared_ptr<ITaskScheduler> create_task_scheduler();

}  // namespace sung
