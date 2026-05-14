#include "thread_pool.h"

ThreadPool::ThreadPool(size_t threads) : stop_(false)
{
    for (size_t i = 0; i < threads; ++i)
    {
        workers_.emplace_back([this]
                              {
            while (true) 
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queue_mutex_);
                    // 等待任务到来 或 线程池关闭
                    condition_.wait(lock, [this] {
                        return stop_ || !tasks_.empty();
                    });
                    // 线程池关闭且没任务了，退出
                    if (stop_ && tasks_.empty()) return;
                    // 从队列取一个任务
                    task = std::move(tasks_.front());
                    tasks_.pop();
                }
                task();  // 执行任务
            } });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        stop_ = true;
    }
    condition_.notify_all(); // 唤醒所有线程
    for (std::thread &worker : workers_)
    {
        worker.join(); // 等待所有线程退出
    }
}

void ThreadPool::submit(std::function<void()> task)
{
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        tasks_.push(std::move(task));
    }
    condition_.notify_one(); // 唤醒一个等待线程
}